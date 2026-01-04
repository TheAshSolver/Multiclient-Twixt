Server can manage many rooms. Each room consists of a game. Each client will give a game id to indicate the game they are joining. 

How to handle the game side of this

So the server doesn't deal with game logic at all. It only deals with passing messages. 
Client has to check for valid move, before sending the move out.

Server will simply receive move and send it out.  
Once a client joins, the server will check if there is already another client connected to the same game.


Each game will be a struct, with a turn variable, the socket of the red player and the socket of the black player. 
