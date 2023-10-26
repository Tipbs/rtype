---
Created: <2023-10-14>
Authors: ["@TAILLAN-Lucas"]
Contributors:
- "@Nestyles"
- "@Tipbs"
- "@TAILLAN-Lucas"
- "@Draze999"
- "@MizuriGit"
---

## UDP-Server-Client Communication Protocol for R-Type

# Summary

This document describes the UDP-based server-client communication protocol used in this R-Type.
The UDP-Server-Client Communication Protocol is designed for use in a one-to-four multiplayer R-Type game. It enables clients to connect to a central server and exchange data, primarily related to game state synchronization.

# Motivation

We are a group of 3rd Year students at Epitech aiming to hone our skills in Network, Game, and Architectural programming. Developping a multiplayer game using Entity Component System Architectural pattern (ECS) is a really good way to attain our objectives. The project here is to make a R-Type game, which basically is a Shoot'Em Up-type of game in which your goal is to shoot enemies and avoid attacks and/or obstacles.

# Protocol Overview

## 1. Communication
    - Communication between the Server and its clients occurs over UDP sockets using Boost.Asio.
    - The server listens on a specific port (default: 5000) for incoming client connections.
    - The client connects to the server from this port and the IP address of the machine's server.

## 2. Message Format
    - Messages are serialized using the Boost.Serialization library and are transmitted as binary data.
    - The server sends asynchronously a serialized vector of "NetEnd" structures and is waiting to receive a serialized "UserCmd" structure from every clients.
    - The clients must send a serialized "UserCmd" structure to the server and asynchronously receive a serialized vector of "NetEnd" structures.

## 3. Message Types and Structures
    The server communicates with its clients using multiple serialized structures.
    1. UserCmd
        - Sent by clients to provide user input commands for the game.
        - The UserCmd structure contains all the data of a client's own player. It contains the relative position of the player in the vector "moved",
a boolean attacking state "attacking", and a float containing the scale of the attack "attackState".
        - The clients must send the UserCmd structure every "tick" to update its players position in the server. It must also be serialized using the Boost.Serialization method.
        - The structure is provided in the "shared" folder located at the root of the project.
    2. NetEnt
        - Sent by the server to the clients to synchronize the ECS' game entities.
        - The NetEnt structure contains all the data of an entity from the server-side ECS. It contains the ID of the entity "id", an enum containing the type of the entity "EntityType", the position of the entity in a vector "pos", and a bool containing the attacking state of an entity.
        - The server sends a vector of NetEnt containing all the game entities to the client. The clients must be able to receive a vector of NetEnt structures from the Boost.Serialization method.
        - The structure is provided in the "shared" folder located at the root of the project.
    3. EntityType
        - Enum providing the type of an entity for the NetEnt structure.
        - "Player": is the type of a player entity.
        - "Enemy": is the type of an Enemy entity.
        - "Win" and "Lose": When a level is finished, the server send a single NetEnt structure to all clients, containing the EntityType "Win" or "Lose", indicating the end of the game.
    4. PlayerId
        - Sent by the server when a client first makes contact with it. The client has to send a 1 byte message indicating to the server that it wants to receive the PlayerId structure. 
        - The structure is provided in the "shared" folder located at the root of the project. It contains the ID of the client's own player entity.

## 4. Ticks
        - Datas are sent as a tick system. The server sends a NetEnt structures tick every 50 milliseconds, making it 20 ticks per seconds.
        - Clients are the same. They are meant to send 20 ticks of UserCmd structure per seconds.
        - Ticks are meant to compensate for the UDP Protocol System of sending packets with the lack of transmission control. Sometimes packets can be lost or unreceived, so sending 20 tick per seconds make the loss of packets a little less concerning.

# Alternatives

We considered using an Enum to define every type of component of the ECS so that it makes it possible to just send every "sparse_array" of the ECS Architecture. The problem with this method is that it would not have been possible to use the Boost.Serialization library to serialize our packets, which in return would have made some security issues over the sent and received data, that would be vulnerable to buffer overflows or injection attacks.

# References

Boost.Asio Library: https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio.html
Boost.Serialization: https://www.boost.org/doc/libs/1_83_0/libs/serialization/doc/tutorial.html
