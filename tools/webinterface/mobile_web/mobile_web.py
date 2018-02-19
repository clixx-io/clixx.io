import os
import tornado.ioloop
import tornado.web

s = {"First" : {"1":{"Text":"Subdiv",
                     "Page":"Sometext"}
               ,"2":"AnotherDiv"}}


class MainHandler(tornado.web.RequestHandler):
    def get(self):
        storm_chasers = ["TIV 1", "TIV 2"]
       
        self.render("home.html", 
                    storm_chasers=storm_chasers)                   


handlers = [
    (r"/", MainHandler),
]

settings = dict(
        template_path=os.path.join(os.path.dirname(__file__), "templates"),
)               

application = tornado.web.Application(handlers, **settings)

if __name__ == "__main__":
    
    #    application.listen(8888)
    # tornado.ioloop.IOLoop.instance().start()



