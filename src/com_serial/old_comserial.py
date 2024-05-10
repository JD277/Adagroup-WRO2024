import serial,time

if __name__ == '__main__':
    
    print('Corriendo, CTRL-C para salir.')
    with serial.Serial("/dev/ttyACM1", 9600, timeout=1) as arduino:
        time.sleep(0.1)
        if arduino.isOpen():
            print("{} conectado!".format(arduino.port))
            try:
                while True:
                    cmd=input("Ingresar Comando : ")
                    arduino.write(cmd.encode())
                    time.sleep(0.1)
                    while arduino.inWaiting()==0: pass
                    if  arduino.inWaiting()>0: 
                        res=arduino.readline().decode().rstrip().split(",")
                        res.pop()

                        arduino.flushInput()

                        for i in range(0, len(res)):
                            res[i] = int(res[i])
                            
    
                        print(str(res))

            except KeyboardInterrupt:
                print("Interrupción por teclado.")