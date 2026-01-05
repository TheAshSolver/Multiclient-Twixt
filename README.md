# MULTICLIENT TWIXT GAME #

An implementation of the classic twixt game in multi-client format implemented in pure C and socket programming.

## How to run

First git clone both the files. Then
```
cd server
make
./app

 ```

 In another terminal

 ``` 
 cd client
 make 
 ./app
 <--your game id-->
 ```

 Repeat the same for another client using same game_id to join. 

 Whoever joins first is the red player and whoever joins second is the black player.

 ## Features

Server logs when client joins and shuts down.

In each client, there is a 24 x 24 grid to display the game state. A point becomes colored if an edge is formed. There is no support for removing edges. 