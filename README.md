ECE382_Lab5
===========

#Day 1
##Questions
1)How long will it take the timer to roll over?   
a)64.53 millisecond   
2)How long does each timer count last?    
a)1 microsecond   

![alt test](http://i47.photobucket.com/albums/f189/erik_thompson2/lab5pic_zps2ab53bec.png)

##IR Data Packets
*New Pictures of data analyzer pending (I destroyed my phone with the pictures)

| Pulse                      | Duration (ms) | Timer A counts |
|----------------------------|---------------|----------------|
| Start   logic 0 half-pulse | 9.062         | 8.971          |
| Start logic 1 half-pulse   | 4.453         | 4.419          |
| Data 1 logic 0 half-pulse  | 0.5937        | 0.592          |
| Data 1 logic 1 half-pulse  | 1.656         | 1.641          |
| Data 0 logic 0 half-pulse  | 0.5936        | 0.594          |
| Data 0 logic 1 half-pulse  | 0.5625        | 0.521          |
| Stop logic 0 half-pulse    | 0.6093        | 0.599          |
| Stop logic 1 half-pulse    | 36.406        | 39.27          |


| Button | code (not including start and   stop bits) | Hex      |
|--------|--------------------------------------------|----------|
| 0      | 0010 0000 1101 1111 0000 1000 1111 0111    | 20DF08F7 |
| 1      | 0010 0000 1101 1111 1000   1000 0111 0111  | 20DF8877 |
| 2      | 0010 0000 1101 1111 0100 1000 1011 0111    | 20DF48B7 |
| 3      | 0010 0000 1101 1111 1100   1000 0011 0111  | 20DFC837 |
| Power  | 0010 0000 1101 1111 0001 0000 1110 1111    | 20DF10EF |
| Vol +  | 0010 0000 1101 1111 0100   0000 1011 1111  | 20DF40BF |
| Vol -  | 0010 0000 1101 1111 1100 0000 0011 1111    | 20DFC03F |
| Ch +   | 0010 0000 1101 1111 0000   0000 1111 1111  | 20DF00FF |
| Ch -   | 0010 0000 1101 1111 1000 0000 0111 1111    | 20DF807F |


|       | Data 1, logic 1 half-pulse | Data 0, logic 0, half pulse | Data 0, logic 1 half pulse |
|-------|----------------------------|-----------------------------|----------------------------|
|       | 1.641                      | 0.595                       | 0.53                       |
|       | 1.641                      | 0.594                       | 0.523                      |
|       | 1.636                      | 0.597                       | 0.528                      |
|       | 1.638                      | 0.593                       | 0.525                      |
|       | 1.639                      | 0.593                       | 0.519                      |
|       | 1.642                      | 0.599                       | 0.528                      |
|       | 1.64                       | 0.597                       | 0.525                      |
|       | 1.633                      | 0.592                       | 0.521                      |
|       | 1.642                      | 0.592                       | 0.527                      |
|       | 1.635                      | 0.594                       | 0.522                      |
| Mean  | 1.6387                     | 0.5946                      | 0.5248                     |
| StDev | 0.002968164                | 0.002244994                 | 0.003340659                |

#Day 2
##Required Functionality
