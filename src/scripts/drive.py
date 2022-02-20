###### CONTROLS: ######
# W, A, S, D controls (ask any real gamer)
# k = go slower ; l = go faster
# n = STOP PROGRAM 


import serial
import time
import readchar


def rover_move(go_left, go_right, go_forward, go_backward, s_inc):
    ## Generates suspension command
    ## Produces Left and Right motor values
    ## 0.5 is stop, 0.0 is backwards, 1.0 is forwards        
    ## s_inc = Speed increment
    ## keep s_inc between 0 and 0.5 
    
    leftm = 0.5
    rightm = 0.5
    
    # Go Left
    if(go_left and not go_right and not go_forward and not go_backward):
        #print("Turning Left")
        leftm = leftm - s_inc 
        rightm = rightm + s_inc
    
    # Go Right
    elif(not go_left and go_right and not go_forward and not go_backward):
        #print("Turning Right")
        leftm = leftm + s_inc
        rightm = rightm - s_inc
    
    # Go Forward
    elif(not go_left and not go_right and go_forward and not go_backward):
        #print("Going Forward")
        leftm = leftm + s_inc
        rightm = rightm + s_inc
    
    # Go Backward    
    elif(not go_left and not go_right and not go_forward and go_backward):
        #print("Going Backward")
        leftm = leftm - s_inc
        rightm = rightm - s_inc
    
    # Go Forward and Left
    elif(go_left and not go_right and go_forward and not go_backward):
        #print("Going Forward and Left")
        leftm = leftm + s_inc * 0.05
        rightm = rightm + s_inc
    
    # Go Forward and Right
    elif(not go_left and go_right and go_forward and not go_backward):
        #print("Going Forward and Right")
        leftm = leftm + s_inc
        rightm = rightm + s_inc * 0.05

    # Go Backward and Left
    elif(go_left and not go_right and not go_forward and go_backward):
        #print("Going Backward and Left")
        leftm = leftm - s_inc * 0.05
        rightm = rightm - s_inc
    
    # Go Backward and Right
    elif(not go_left and go_right and not go_forward and go_backward):
        #print("Going Backward and Right")
        leftm = leftm - s_inc
        rightm = rightm - s_inc * 0.05
        
    
    if(go_left or go_right or go_forward or go_backward):
        sus_cmd = 'g,' + str(leftm) + ',' + str(rightm) + '\n'
    else:
        #print("Rover Stopped")
        sus_cmd = 'n'

    return(sus_cmd)
    
    

## Setting up the suspension Arduino
# Baud rates for each Arduino
sus_baud = 115200 # Suspension

# Jetson USB port assignment for each arduino
sus_port = '/dev/ttyACM0'

# Serial Communication port assignment for each Arduino
sus_ard = serial.Serial(sus_port, sus_baud) # Supspension Arduino setup

# Gives time for Arduinos to start up (necessary)
print("Allowing for Arduino to boot...")
time.sleep(3)

# Printing instructions
print("\n[==============  Use WASD to control Rover  ==============]")
print("|=======  k = decrease speed ; l = increase speed  =======|")
print("|==================  n = exit program  ===================|")
print("[===========  !** press SPACE BAR to STOP **!  ===========]\n")

pressed_key = 0
go_forward = False
go_backward = False
go_left = False
go_right = False
s = 0.05 # speed change resolution
s_inc = 0.0 # Current speed increment

driving = True
try:
    while(driving):

        pressed_key = readchar.readchar()
        
        ## Forwards
        if pressed_key == 'w':
            go_forward = True
            go_backward = False
        else:
            go_forward = False

        ## Backwards    
        if pressed_key == 's':
            go_forward = False
            go_backward = True
        else:
            go_backward = False

        ## Do nothing if told to go forwards and backwards
        if (pressed_key == 'w' and pressed_key == 's'):
            go_forward = False
            go_backward = False


        ## Left    
        if pressed_key == 'a':
            go_left = True
            go_right = False
        else:
            go_left = False
            
        ## Right
        if pressed_key == 'd':
            go_left = False
            go_right = True
        else:
            go_right = False

        ## Do nothing if told to go left and right
        if (pressed_key == 'a' and pressed_key == 'd'):
            go_left = False
            go_right = False


        ## STOP
        if pressed_key == ' ':
            go_forward = False
            go_backward = False
            go_left = False
            go_right = False
            
            
        ## Speed Control
        # Increase Speed
        if pressed_key == 'l':
            s_inc = s_inc + s

        # Decrease Speed
        if pressed_key == 'k':
            s_inc = s_inc - s

        print("Speed: ", s_inc)
            
        if(s_inc == 0.0):
            go_forward = False
            go_backward = False
            go_left = False
            go_right = False
        elif(s_inc > 5.0):
            s_inc = 5.0
        elif(s_inc < -5.0):
            s_inc = -5.0
            
            
        # Create suspension command string
        sus_cmd = pressed_key  
        
        # Send string to arduino
        sus_ard.write(sus_cmd.encode('utf8')) # Send string to Suspension Arduino
        time.sleep(.1)
 
        if pressed_key == 'n':
            driving = False
              
except:
    pass


sus_cmd = pressed_key
sus_ard.write(sus_cmd.encode('utf8')) # Send string to Suspension Arduino
time.sleep(.1)     



