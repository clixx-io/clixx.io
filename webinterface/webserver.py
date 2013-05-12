import tornado.ioloop
import tornado.web
import tornado.auth
from tornado import httpclient

from mako import exceptions
from mako.lookup import TemplateLookup
from mako.template import Template

import os

root = os.path.dirname(os.path.realpath(__file__))
# print "Directory is %s" % root
template_root = os.path.join(root, 'templates')
blacklist_templates = ('layouts',)
template_lookup = TemplateLookup(input_encoding='utf-8',
                                 output_encoding='utf-8',
                                 encoding_errors='replace',
                                 directories=[template_root])

def render_template(filename):
    if os.path.isdir(os.path.join(template_root, filename)):
        filename = os.path.join(filename, 'index.html')
    else:
        filename = '%s.html' % filename
    if any(filename.lstrip('/').startswith(p) for p in blacklist_templates):
        raise httpclient.HTTPError(404)
    try:
        return template_lookup.get_template(filename).render()
    except exceptions.TopLevelLookupException:
        raise httpclient.HTTPError(404)

class DefaultHandler(tornado.web.RequestHandler):
    def get_error_html(self, status_code, exception, **kwargs):
        if hasattr(exception, 'code'):
            self.set_status(exception.code)
            if exception.code == 500:
                return exceptions.html_error_template().render()
            return render_template(str(exception.code))
        return exceptions.html_error_template().render()

class MainHandler(DefaultHandler):
    def get(self, filename):
        mylookup = TemplateLookup(directories=['./templates'], output_encoding='utf-8', encoding_errors='replace')
        mytemplate = mylookup.get_template(filename + '.txt')
        self.write(mytemplate.render(data="world"))

class Main(tornado.web.RequestHandler):
        def get_current_user(self):
            return self.get_secure_cookie("user")

        def get(self):
            mylookup = TemplateLookup(directories=['./templates'], output_encoding='utf-8', encoding_errors='replace')
            mytemplate = mylookup.get_template('index.txt')
            if not self.current_user:
                # self.redirect("/login")
                self.write(mytemplate.render(user="None"))
                return

            self.write(mytemplate.render(user=self.current_user))

def do_auth(name, password):
  return True
  with open(auth_file) as f:
    lines = f.readlines()
  for l in lines:
    u,s,h = l.strip().split(':')
    if name == u:
      s5 = hashlib.sha512()
      s5.update(s.decode('hex'))
      s5.update(password)
      if h == s5.hexdigest():
        return True
  return False
  
class Login(Main):
  def get(self):
    mylookup = TemplateLookup(directories=['./templates'], output_encoding='utf-8', encoding_errors='replace')
    mytemplate = mylookup.get_template('login.txt')
    
    self.write(mytemplate.render(user=self.current_user))
    return

    Template("hello ${data}!").render(data="world")
    self.render(Template("hello ${data}!").render(data="world"))  
    self.write('<!DOCTYPE html><html><body><form action="/login" method="post">'
               'Name: <input type="text" name="name"><br />'
               'Password: <input type="password" name="password"><br />'
               '<input type="submit" value="Sign in">'
               '</form></body></html>')

  def post(self):
    if do_auth(self.get_argument("name"), self.get_argument("password")):
      self.set_secure_cookie("user", self.get_argument("name"))
      self.redirect("/")
    else:
      self.redirect("/login")

class BaseHandler(tornado.web.RequestHandler):

  def get_login_url(self):
    return u"/login"

  def get_current_user(self):
    user_json = self.get_secure_cookie("user")
    if user_json:
      return tornado.escape.json_decode(user_json)
    else:
      return None
      
class LoginHandler(BaseHandler):
  def get(self):
    self.write('<html><body><form action="/login" method="post">'
               'Name: <input type="text" name="name"><br />'
               'Password: <input type="password" name="password"><br />'
               '<input type="submit" value="Sign in">'
               '</form></body></html>')

  def post(self):
    if do_auth(self.get_argument("name"), self.get_argument("password")):
      self.set_secure_cookie("user", self.get_argument("name"))
      self.redirect("/")
    else:
      self.redirect("/login")

class RegisterHandler(LoginHandler):

  def get(self):
    self.render("register.html", next=self.get_argument("next","/"))

  def post(self):
    email = self.get_argument("email", "")

    already_taken = self.application.syncdb['users'].find_one( { 'user': email } )
    if already_taken:
      error_msg = u"?error=" + tornado.escape.url_escape("Login name already taken")
      self.redirect(u"/login" + error_msg)


    password = self.get_argument("password", "")
    hashed_pass = bcrypt.hashpw(password, bcrypt.gensalt(8))

    user = {}
    user['user'] = email
    user['password'] = hashed_pass

    auth = self.application.syncdb['users'].save(user)
    self.set_current_user(email)

    self.redirect("hello")

class LogoutHandler(BaseHandler):
    def get(self):
        self.clear_cookie("user")
        self.redirect("/")

application = tornado.web.Application([
        (r"/", Main),
        (r"/login", Login),
        (r"/logout", LogoutHandler),
        (r"/images/(.*)",tornado.web.StaticFileHandler, {"path": "./media/images"},),
        (r"/fonts/(.*)",tornado.web.StaticFileHandler, {"path": "./media/fonts"},),
        (r"/css/(.*)",tornado.web.StaticFileHandler, {"path": "./css"},),
        (r"/lib/(.*)",tornado.web.StaticFileHandler, {"path": "./lib"},),
        (r"/js/(.*)",tornado.web.StaticFileHandler, {"path": "./lib/js"},),
        (r'^/(.*)$', MainHandler),
        ],debug=True, cookie_secret="61oETzKXQAGaYdkL5gEmGeJJFuYh7EQnp2XdTP1o/Vo=",
        static_path=os.path.join(root, 'static'))

if __name__ == "__main__":

    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()

