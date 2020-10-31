from datetime import datetime
import json
import time
import random
import string
import requests
import treq
from klein import Klein
from twisted.internet import task
from twisted.internet import threads
from twisted.web.server import Site
from twisted.internet import reactor, endpoints

app = Klein()

def test(y):
    print("test called at {datetime.now().isoformat()}")
    x = requests.get("http://www.example.com")
    time.sleep(10)

    return json.dumps([{
        "time": datetime.now().isoformat(),
        "text": x.text[:10],
        "arg": y
    }])

@app.route('/<string:y>',methods = ['GET'])
def index(request, y):
    return threads.deferToThread(test, y)

def send_requests():
    # send 3 concurrent requests
    rand_letter = random.choice(string.ascii_letters)
    for i in range(3):
        y = rand_letter + str(i)
        print("request send at {datetime.now().isoformat()}")
        d = treq.get('localhost:8080')
        d.addCallback(treq.content)
        #d.addCallback(lambda r: print(r.decode()))

loop = task.LoopingCall(send_requests)
loop.start(15) # repeat every 15 seconds

reactor.suggestThreadPoolSize(3)

# disable unwanted logs
# app.run("localhost", 8080)

# this way reactor logs only print calls
web_server = endpoints.serverFromString(reactor, "tcp:8080")
web_server.listen(Site(app.resource()))
reactor.run()
