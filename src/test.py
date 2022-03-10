import requests
import time

while True:
    t = time.time()
    r = requests.get("http://localhost:5000")
    ct = (time.time()-t)*1000
    print("%d ms" % ct)