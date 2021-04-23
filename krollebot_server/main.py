import eventlet
import socketio
import serial
import atexit # lukke for serial port når programmet lukker

# websocket server
sio = socketio.Server(cors_allowed_origins='*')
app = socketio.WSGIApp(sio)

# serial
# ser = serial.Serial('COM3')

@sio.event
def connect(sid, environ):
    print('connect ', sid)

@sio.event
def krollebot_message(sid, data):
    ser.write(str(data).encode())
    print('Skriver til Arduino ', data)

@sio.event
def disconnect(sid):
    print('disconnect ', sid)

def exit_handler():
    # ser.close()
    print('Programmet lukkes')

if __name__ == '__main__':
    atexit.register(exit_handler)
    eventlet.wsgi.server(eventlet.listen(('localhost', 5001)), app)
    print("Hej")
