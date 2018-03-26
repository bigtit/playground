import socket
import threading
import subprocess
import sys
import os
import signal
import time

action_number = 0
action_list = []

class ThreadedServer(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))

    def listen(self):
        self.sock.listen(1)
        while True: # when client comes in
            client, address = self.sock.accept()
            client.settimeout(60)
            threading.Thread(target = self.listenToClient,args = (client,address)).start()

    def listenToClient(self, client, address):
        size = 1024
        while True:
            try:
                data = client.recv(size) # block
                if data:
                    # Set the response to echo back the recieved data 
                    respond = list(data)
                    print(data == bytes(respond))
                    respond[0] += 1
                    global action_number
                    global action_list
                    # for each action, emulator will send its number to python script
                    if respond[0] == 10:
                        ++action_number
                        action_list.append(respond[1]) # state
                        action_list.append(respond[2]) # action
                    elif respond[0] == 11:
                        --action_number
                    elif respond[0] == 12: # timeout, do next experiment
                        --action_number
                else:
                    raise error('Client disconnected')
            except:
                # print('client closed')
                client.close()
                return False

#if __name__ == "__main__":
#    ThreadedServer('',5005).listen()

# detect connectivity using ADB ping
# actually we dont need to detect ping when connecting to malicious eNB
# only need to detect it several seconds after UE attach to benign eNB
def detect_conn():
    ret = False
    output = b''
    p = subprocess.Popen('exec ping -i 0.3 -c 4 8.8.8.8', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE) 
    try:
        p.wait(timeout=4) # prevent ping from hanging forever
    except subprocess.TimeoutExpired:
        p.stdout.flush()
        output = p.stdout.read()
        #print(output)
        p.kill()
        ret = output.count(b'time=') >= 4
    finally:
        output = p.stdout.read()
        #print(output)
        ret = output.count(b'time=') >= 4

    return ret

# kill emulator with signal
def kill_emu(x):
    p = subprocess.Popen('exec ps -A', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out,err = p.communicate()
    for line in out.splitlines():
        if x in line:
            os.kill(int(line.split(None,1)[0]), signal.SIGINT) # safe Ctrl+C to emu

def start_enb(x=0):
    return True
def start_ue():
    return True


# return action idx that causes availability problem
# x: action number
def run_episode_av(x):
    stop_ue()
    stop_enb()
    i = 0
    j = 0
    c = x
    while 1<c and j<x:
        c = (c+1)//2
        start_ue()
        sleep(20)

        start_enb(c+j) # intercept
        stop_enb()
        sleep(10)
        if detect_conn(): j += c
        ++i
        stop_ue()
    if 1==c or j==x: return j
    return -1
    
def req_action_number():
    # setup socket
    return 0

#print(detect_conn())
# how to replay a seq
# seq info was saved inside emu, only integer
# if current state != expected state in saved seq, experiment fails
# we only need to turn off eNB, not EPC
# since we use socket to communicate with emu, it does not matter where the python script is
# thus we put the script in eNB side, also ADB should be connected with eNB side
# thread model: main thread handles experiment, sub thread handles communication with emulator

def run_experiment_av():
    # init
    global action_number
    action_number = 0
    stop_enb()
    stop_ue()

    start_ue() # wait for attach
    sleep(20)
    start_enb()
    # wait for attach
    sleep(20)

    stop_enb()
    if not detect_conn():
        run_episode_av(action_number)
    pass

def run_experiment_cf():
    pass

def main():
    ts = ThreadedServer('', 5005)
    threading.Thread(target = ts.listen,args = ()).start()
    # main thread
    # run_experiment
    while True:
	    time.sleep(2)
	    print("main thread")

if __name__ == "__main__":
    main()