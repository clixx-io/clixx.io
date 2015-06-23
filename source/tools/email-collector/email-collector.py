#!/usr/bin/python
import os
import subprocess
import sys, platform, glob
import datetime
import time
import shutil
import logging
import logging.config
import ConfigParser
from optparse import OptionParser
import getpass, poplib
from email import parser
import base64
import dbfpy.dbf
from circuits import Event, Component, Timer
import tempfile
from email.Header import decode_header
import email
from base64 import b64decode
from email.Parser import Parser as EmailParser
from email.utils import parseaddr
from StringIO import StringIO

# Insert your mail settings here 
mailserver = ""
mailuser   = ""
mailpwd    = ""

config_filename = "email-collector.ini"

task_log_name = "email-collector.log"

# Logging setup
logging.basicConfig(format='%(asctime)s %(message)s',filename=task_log_name,level=logging.DEBUG)
logger = logging.getLogger(__name__)

# Configuration variables
config = ConfigParser.SafeConfigParser()

timestartfrom = None
timestopat = None

purge_after_print = False
archive_directory = "printed"


class NotSupportedMailFormat(Exception):
    pass

def parse_attachment(message_part):
    content_disposition = message_part.get("Content-Disposition", None)
    if content_disposition:
        dispositions = content_disposition.strip().split(";")
        if bool(content_disposition and dispositions[0].lower() == "attachment"):

            file_data = message_part.get_payload(decode=True)
            # Used a StringIO object since PIL didn't seem to recognize
            # images using a custom file-like object
            attachment = StringIO(file_data)
            attachment.content_type = message_part.get_content_type()
            attachment.size = len(file_data)
            attachment.name = None
            attachment.create_date = None
            attachment.mod_date = None
            attachment.read_date = None
            
            for param in dispositions[1:]:
                name,value = param.split("=")
                name = name.lower()

                if name == "filename":
                    attachment.name = value
                elif name == "create-date":
                    attachment.create_date = value  #TODO: datetime
                elif name == "modification-date":
                    attachment.mod_date = value #TODO: datetime
                elif name == "read-date":
                    attachment.read_date = value #TODO: datetime
            return attachment

    return None
 
def parse(f):
    """
    Parse the email and return a dictionary of relevant data.
    """

    p = EmailParser()

    msgobj = email.message_from_file(f)

    attachments = []
    body = None 
    html = None 
    for part in msgobj.walk():
        attachment = parse_attachment(part)
        if attachment:
            attachments.append(attachment)
        elif part.get_content_type() == "text/plain":
            if body is None:
                body = ""
            body += part.get_payload(decode=True)

        elif part.get_content_type() == "text/html":
            if html is None:
                html = ""
            html += unicode(
                part.get_payload(decode=True),
                part.get_content_charset(),
                'replace'
            ).encode('utf8','replace')
    return {
#        'subject' : subject,
        'body' : body,
        'html' : html,
        'from' : parseaddr(msgobj.get('From'))[1], # Leave off the name and only return the address
        'to' : parseaddr(msgobj.get('To'))[1], # Leave off the name and only return the address
        'attachments': attachments,
    }

import sys, os, re, StringIO
import email, mimetypes

invalid_chars_in_filename='<>:"/\\|?*\%\''+reduce(lambda x,y:x+chr(y), range(32), '')
invalid_windows_name='CON PRN AUX NUL COM1 COM2 COM3 COM4 COM5 COM6 COM7 COM8 COM9 LPT1 LPT2 LPT3 LPT4 LPT5 LPT6 LPT7 LPT8 LPT9'.split()

# email address REGEX matching the RFC 2822 spec from perlfaq9
#    my $atom       = qr{[a-zA-Z0-9_!#\$\%&'*+/=?\^`{}~|\-]+};
#    my $dot_atom   = qr{$atom(?:\.$atom)*};
#    my $quoted     = qr{"(?:\\[^\r\n]|[^\\"])*"};
#    my $local      = qr{(?:$dot_atom|$quoted)};
#    my $domain_lit = qr{\[(?:\\\S|[\x21-\x5a\x5e-\x7e])*\]};
#    my $domain     = qr{(?:$dot_atom|$domain_lit)};
#    my $addr_spec  = qr{$local\@$domain};
# 
# Python's translation

atom_rfc2822=r"[a-zA-Z0-9_!#\$\%&'*+/=?\^`{}~|\-]+"
atom_posfix_restricted=r"[a-zA-Z0-9_#\$&'*+/=?\^`{}~|\-]+" # without '!' and '%'
atom=atom_rfc2822
dot_atom=atom  +  r"(?:\."  +  atom  +  ")*"
quoted=r'"(?:\\[^\r\n]|[^\\"])*"'
local="(?:"  +  dot_atom  +  "|"  +  quoted  +  ")"
domain_lit=r"\[(?:\\\S|[\x21-\x5a\x5e-\x7e])*\]"
domain="(?:"  +  dot_atom  +  "|"  +  domain_lit  +  ")"
addr_spec=local  +  "\@"  +  domain

email_address_re=re.compile('^'+addr_spec+'$')

class Attachment:
    def __init__(self, part, filename=None, type=None, payload=None, charset=None, content_id=None, description=None, disposition=None, sanitized_filename=None, is_body=None):
        self.part=part          # original python part
        self.filename=filename  # filename in unicode (if any) 
        self.type=type          # the mime-type
        self.payload=payload    # the MIME decoded content 
        self.charset=charset    # the charset (if any) 
        self.description=description    # if any 
        self.disposition=disposition    # 'inline', 'attachment' or None
        self.sanitized_filename=sanitized_filename # cleanup your filename here (TODO)  
        self.is_body=is_body        # usually in (None, 'text/plain' or 'text/html')
        self.content_id=content_id  # if any
        if self.content_id:
            # strip '<>' to ease searche and replace in "root" content (TODO) 
            if self.content_id.startswith('<') and self.content_id.endswith('>'):
                self.content_id=self.content_id[1:-1]

def getmailheader(header_text, default="ascii"):
    """Decode header_text if needed"""
    try:
        headers=email.Header.decode_header(header_text)
    except email.Errors.HeaderParseError:
        # This already append in email.base64mime.decode()
        # instead return a sanitized ascii string
        # this faile '=?UTF-8?B?15HXmdeh15jXqNeVINeY15DXpteUINeTJ9eV16jXlSDXkdeg15XXldeUINem15PXpywg15TXptei16bXldei15nXnSDXqdecINek15zXmdeZ?==?UTF-8?B?157XldeR15nXnCwg157Xldek16Ig157Xl9eV15wg15HXodeV15bXnyDXk9ec15DXnCDXldeh15gg157Xl9eR16rXldeqINep15wg15HXmdeQ?==?UTF-8?B?15zXmNeZ?='
        return header_text.encode('ascii', 'replace').decode('ascii')
    else:
        for i, (text, charset) in enumerate(headers):
            try:
                headers[i]=unicode(text, charset or default, errors='replace')
            except LookupError:
                # if the charset is unknown, force default 
                headers[i]=unicode(text, default, errors='replace')
        return u"".join(headers)

def getmailaddresses(msg, name):
    """retrieve addresses from header, 'name' supposed to be from, to,  ..."""
    addrs=email.utils.getaddresses(msg.get_all(name, []))
    for i, (name, addr) in enumerate(addrs):
        if not name and addr:
            # only one string! Is it the address or is it the name ?
            # use the same for both and see later
            name=addr
            
        try:
            # address must be ascii only
            addr=addr.encode('ascii')
        except UnicodeError:
            addr=''
        else:
            # address must match address regex
            if not email_address_re.match(addr):
                addr=''
        addrs[i]=(getmailheader(name), addr)
    return addrs

def get_filename(part):
    """Many mail user agents send attachments with the filename in 
    the 'name' parameter of the 'content-type' header instead 
    of in the 'filename' parameter of the 'content-disposition' header.
    """
    filename=part.get_param('filename', None, 'content-disposition')
    if not filename:
        filename=part.get_param('name', None) # default is 'content-type'
        
    if filename:
        # RFC 2231 must be used to encode parameters inside MIME header
        filename=email.Utils.collapse_rfc2231_value(filename).strip()

    if filename and isinstance(filename, str):
        # But a lot of MUA erroneously use RFC 2047 instead of RFC 2231
        # in fact anybody miss use RFC2047 here !!!
        filename=getmailheader(filename)
        
    return filename

def _search_message_bodies(bodies, part):
    """recursive search of the multiple version of the 'message' inside 
    the the message structure of the email, used by search_message_bodies()"""
    
    type=part.get_content_type()
    if type.startswith('multipart/'):
        # explore only True 'multipart/*' 
        # because 'messages/rfc822' are also python 'multipart' 
        if type=='multipart/related':
            # the first part or the one pointed by start 
            start=part.get_param('start', None)
            related_type=part.get_param('type', None)
            for i, subpart in enumerate(part.get_payload()):
                if (not start and i==0) or (start and start==subpart.get('Content-Id')):
                    _search_message_bodies(bodies, subpart)
                    return
        elif type=='multipart/alternative':
            # all parts are candidates and latest is best
            for subpart in part.get_payload():
                _search_message_bodies(bodies, subpart)
        elif type in ('multipart/report',  'multipart/signed'):
            # only the first part is candidate
            try:
                subpart=part.get_payload()[0]
            except IndexError:
                return
            else:
                _search_message_bodies(bodies, subpart)
                return

        elif type=='multipart/signed':
            # cannot handle this
            return
            
        else: 
            # unknown types must be handled as 'multipart/mixed'
            # This is the peace of code could probably be improved, I use a heuristic : 
            # - if not already found, use first valid non 'attachment' parts found
            for subpart in part.get_payload():
                tmp_bodies=dict()
                _search_message_bodies(tmp_bodies, subpart)
                for k, v in tmp_bodies.iteritems():
                    if not subpart.get_param('attachment', None, 'content-disposition')=='':
                        # if not an attachment, initiate value if not already found
                        bodies.setdefault(k, v)
            return
    else:
        bodies[part.get_content_type().lower()]=part
        return
    
    return

def search_message_bodies(mail):
    """search message content into a mail"""
    bodies=dict()
    _search_message_bodies(bodies, mail)
    return bodies

def get_mail_contents(msg):
    """split an email in a list of attachments"""

    attachments=[]

    # retrieve messages of the email
    bodies=search_message_bodies(msg)
    # reverse bodies dict
    parts=dict((v,k) for k, v in bodies.iteritems())

    # organize the stack to handle deep first search
    stack=[ msg, ]
    while stack:
        part=stack.pop(0)
        type=part.get_content_type()
        if type.startswith('message/'): 
            # ('message/delivery-status', 'message/rfc822', 'message/disposition-notification'):
            # I don't want to explore the tree deeper her and just save source using msg.as_string()
            # but I don't use msg.as_string() because I want to use mangle_from_=False 
            from email.Generator import Generator
            fp = StringIO.StringIO()
            g = Generator(fp, mangle_from_=False)
            g.flatten(part, unixfrom=False)
            payload=fp.getvalue()
            filename='mail.eml'
            attachments.append(Attachment(part, filename=filename, type=type, payload=payload, charset=part.get_param('charset'), description=part.get('Content-Description')))
            
        elif part.is_multipart():
            # insert new parts at the beginning of the stack (deep first search)
            stack[:0]=part.get_payload()
        else:
            payload=part.get_payload(decode=True)
            charset=part.get_param('charset')
            filename=get_filename(part)
                
            disposition=None
            if part.get_param('inline', None, 'content-disposition')=='':
                disposition='inline'
            elif part.get_param('attachment', None, 'content-disposition')=='':
                disposition='attachment'
                
            attachments.append(Attachment(part, filename=filename, type=type, payload=payload, charset=charset, content_id=part.get('Content-Id'), description=part.get('Content-Description'), disposition=disposition, is_body=parts.get(part)))

    return attachments

def decode_text(payload, charset, default_charset):
    if charset:
        try: 
            return payload.decode(charset), charset
        except UnicodeError:
            pass

    if default_charset and default_charset!='auto':
        try: 
            return payload.decode(default_charset), default_charset
        except UnicodeError:
            pass
        
    for chset in [ 'ascii', 'utf-8', 'utf-16', 'windows-1252', 'cp850' ]:
        try: 
            return payload.decode(chset), chset
        except UnicodeError:
            pass

    return payload, None

def check_messages(M):

    import email
    messagelist = []

    numMessages = len(M.list()[1])
    for i in range(numMessages):

        logger.debug("Retrieving %d" % (i+1))

        print("Retrieving message ", i+1)

        m_info = {}

        number = 1
        raw = ""
        try:
            #------Check if email number is valid----------------------
            (numMsgs, totalSize) = M.stat()
            logger.debug("Total number of server messages: %d" % numMsgs)                
            logger.debug("Total size   of server messages: %d" % totalSize)
            if  number>numMsgs:
                print(200, "\nSorry - there aren't that many messages in your inbox\n")
                return False
            else:
                (server_msg, body, octets) = M.retr(number)

                logger.debug("Server Message: %s" % server_msg)
                logger.debug("Number of Octets: %s" % octets)

                tempname = tempfile.NamedTemporaryFile(prefix='email_', suffix='.MSG', dir='.', delete=False)
                logger.debug("Writing to temporary file %s",tempname.name)

                text_file = open(tempname.name, "w")
                for line in body:
                    text_file.write(str(line) + '\n')
                    raw += str(line + '\n')

                text_file.close()

            messagelist.append(i+1)

        finally:
            pass

        msg=email.message_from_string(raw)
        attachments=get_mail_contents(msg)

        subject=getmailheader(msg.get('Subject', ''))
        from_=getmailaddresses(msg, 'from')
        from_=('', '') if not from_ else from_[0]
        tos=getmailaddresses(msg, 'to')

    return messagelist

def printemails():

    global purge_after_print,archive_directory

    processed_msgs = []
    
    msglist = glob.glob('message-*.MSG')
    for m in msglist:

        f = open(m)

        print "Processing %s" % m

        p = m[:-4] + '.prn'

        msg=email.message_from_file(f)
        attachments=get_mail_contents(msg)

        subject=getmailheader(msg.get('Subject', ''))
        from_=getmailaddresses(msg, 'from')
        from_=('', '') if not from_ else from_[0]
        tos=getmailaddresses(msg, 'to')
    
        email_file = open(p,'w')
    
        email_file.write('To: %r\n' % (tos, ))
        email_file.write('From: %r\n' % (from_, ))
        email_file.write('Subject: %r\n' % subject)
        email_file.write('\n')

        for a in attachments:


            email_file.write("--------------------------------------------------------\n")
            email_file.write(str(a.charset))                 # the charset (if any) 
            print(str(a.disposition))    		# 'inline', 'attachment' or None
            print(str(a.sanitized_filename)) 	# cleanup your filename here (TODO)  
            email_file.write(str(a.content_id))  		# if any

            if a.is_body == "text/plain":
                email_file.write(str(a.payload))

            else:
                if not a.filename is None: #.lower().endswith('.pdf'):

                    logger.debug("**WRITING ==>> FILENAME== %s" % a.filename)

                    o = open(a.filename.replace(' ','_'),'w')
                    o.write(a.payload)
                    o.close()

        email_file.close()

        f.close()

#    msglist = glob.glob('message-*.prn')
    msglist = glob.glob('*.pdf')
    msglist += glob.glob('*.PDF')

    for m in msglist:

        logger.info("Printing %s" % m)
        os.system('lp \"{0}\"'.format(m))
        
    msglist = glob.glob('*.msg')
    msglist += glob.glob('*.pdf')
    msglist += glob.glob('*.PDF')
    msglist += glob.glob('*.prn')
    for m in msglist:
        if purge_after_print:
            os.remove(m)
        else:
            shutil.move(m,archive_directory)

    return processed_msgs

def delete_emails(M, msgdeletelist):
    """
    Delete emails from off the mail server
    """

    for i in msgdeletelist:

        logger.debug("Deleting %d" % i)

        logger.debug(M.dele(i))

    return

def run():

    M = poplib.POP3(mailserver)
    M.user(mailuser)
    M.pass_(mailpwd)

    updatelist = check_messages(M)
    
    print "Updatelist", updatelist

    printemails()
    
    # delete_emails(M, updatelist)

    M.quit()

    logger.debug("Messages checked")

class App(Component):

    def checkmessages(self):
        """hello Event handler
        Fired once in 5 seconds.
        """
        run()

    def started(self, component):
        """started Event handler
        """
        # Timer(seconds, event, persist=False)
        Timer(20, Event.create("checkmessages"), persist=True).register(self)

if __name__ == "__main__":

    usage = "usage: %prog [options] arg"
    parser = OptionParser(usage)
    parser.add_option("-o", "--once", dest="once",action="store_true",
                      help="Run just one time")

    (options, args) = parser.parse_args()

    txtfiles = []

    logger.info("Program startup")    

    try:

        run()

        if options.once:
            sys.exit(0)

        logger.info("Program startup")

        myApp = App()

        myApp.run()

    except KeyboardInterrupt:

        logger.info("Program shutdown")
        sys.exit(0)

