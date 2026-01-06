Group Members: Tudor Ganshaw, Eviss Wu

Description:  
	A multiplayer trivia game. Clients can connect to a main server, which will then provide all clients with a given question. Each player will then answer the question, which will send their answer to the server. Once all players have answered the question, the server will send the clients the updated scores and a new question.

User Interface:  
For the server, the user will be prompted to select a number of questions, a number of players, and a server name. After inputting this information, they will be redirected to a screen where they will see all the players that join the game. At any point, they can start the game.  
	  
Clients will first have to log in or create an account. Then, each client will connect to a main server, where you’ll be prompted to put a lobby name. Once the lobby creator starts the game, clients will see a number of questions. For each question, a subserver will be forked for every client that will deal with checking answers and updating the score. For that lobby, there’ll be x number of questions to answer, and a lobby leaderboard. After all questions are done, the game will end and clients will see the game leaderboard, before being prompted to go back to the menu screen.

Design:

- File Reading to get a random question and its answers from cache  
- File Writing to store password and username  
- Global leaderboard will be written to a public file  
- Server forks to deal with adding score for each player  
- If client is killed, sends a signal to server so server doesn’t wait for the client’s answers  
- Sockets used to communicate between server and client  
- Memory allocation will be used generally to send packets  
- Structs to store questions for code readability

	Task Delineation:

- Question creation system: Tudor  
- Server-client connection: Eviss  
- Client answer system: Eviss  
- Server answer checking system: Tudor  
- Global score leaderboard: Tudor  
- User authentication system: Eviss  
- Presentation video editing: Tudor  
- Further tasks will be added any of the tasks above prove too challenging for only one person to do

	  
Timeline:

- Server-client connection finished (single client): 1/7/26  
- Cached server database: 1/7/26  
- Question creation system finished: 1/7/26  
- Client Answer system finished: 1/8/26  
- Server answer checking system finished: 1/9/26  
- MVP: 1/12/26  
- User authentication system finished: 1/14/26  
- Global score leaderboard finished: 1/15/26  
- Presentation video finished: 1/16/26

