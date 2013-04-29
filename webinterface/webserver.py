import tornado.ioloop
import tornado.web
from mako import exceptions
from mako.lookup import TemplateLookup
from tornado import httpclient
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
        self.write(mytemplate.render())

class Main(tornado.web.RequestHandler):
        def get_current_user(self):
            return self.get_secure_cookie("user")

        def get(self):
            if not self.current_user:
                # self.redirect("/login")
                mylookup = TemplateLookup(directories=['./templates'], output_encoding='utf-8', encoding_errors='replace')
                mytemplate = mylookup.get_template('index.txt')
                self.write(mytemplate.render())
                return

            username = self.current_user
            #self.render('templates/index.txt')
            self.write('Hi there, '+ username)

class Login(Main):
        def get(self):
#                self.write("Helps if you login")
                self.render('templates/login.txt')
        def post(self):
                self.set_secure_cookie("user", self.get_argument("username"))
                self.write('Hi there, '+ username)
               # self.redirect("/")

application = tornado.web.Application([
        (r"/", Main),
        (r"/login", Login),
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

