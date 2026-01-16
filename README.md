[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
### [Video Link](https://youtu.be/nsEZc0vY_3g)

# Pokemon Trivia

### The Goobers

Tudor Ganshaw, Eviss Wu

### Project Description:

A multiplayer trivia game. Clients can either join a lobby or make a lobby. Then, clients can connect to a main server, which will then provide all clients with a given question. Each player will then answer the question, which will send their answer to the server. Once all players have answered the question, the server will send the clients the updated scores and a new question. The game will end after the lobby's selected question amount are over. The most correct answers out of the clients win.

### Instructions:

Install Guide:

Requirements:
  - Make installed

Clone repo:

  ``` git clone git@github.com:Stuycs-K/p03-final-10f-ganshawt-wue.git```



Launch Codes:

CD into Repo App Folder:

  ``` cd p03-final-10f-ganshawt-wue/app```

Run Server:

  ``` make client && make server && ./server ```

Run Clients:

  ``` ./client ```

Once all clients are connected, the game will automatically start.

How to Play:
- Each client gets the same 10 questions, with multiple choice answers from 1 to 4.
- They must answer these questions to the best of their ability by typing in the correct answer (1,2,3, or 4) and pressing enter.
- Once all clients complete the questions, the game will end and each client will see the leaderboard and how the other clients scored.

### Resources/ References:

[PokeAPI](https://pokeapi.co/docs/v2) was used to cache all Pokemon data.
