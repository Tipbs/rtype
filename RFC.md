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

1. Communication
    - Communication between the Server and its clients occurs over UDP sockets using Boost.Asio.
    - The server listens on a specific port (default: 5000) for incoming client connections.
    - The client connects to the server from this port and the IP address of the machine's server.

2. Message Format
    - Messages are serialized using the Boost.Serialization library and are transmitted as binary data.
    - The server sends asynchronously a serialized vector of "NetEnd" structures and is waiting to receive a serialized "UserCmd" structure from every clients.
    - The clients must send a serialized "UserCmd" structure to the server and asynchronously receive a serialized vector of "NetEnd" structures.

3. Message Types
    3.1. UserCmd
        - Sent by clients to provide user input commands for the game.
        - The structure is provided in the "shared" folder located at the root of the project. It contains the relative position of the client sending the structure and its state.
    3.2. NetEnt
        - Sent by the server to the clients to synchronize the ECS' game entities.
        - The structure is provided in the "shared folder located at the root of the project. It contains the ID of the entity, its relative position and its state.
    3.3. PlayerId
        - Sent by the server when a client first makes contact with it. The client has to send a 1 byte message indicating to the server that it wants to receive the PlayerId structure. 
        - The structure is provided in the "shared" folder located at the root of the project. It contains the ID of the client's own player entity.

4. Ticks
    - Datas are sent as a tick system. The server sends a NetEnt structures tick every 50 milliseconds, making it 20 ticks per seconds.
    - Clients are the same. They are meant to send 20 ticks of UserCmd structure per seconds.
    - Ticks are meant to compensate for the UDP Protocol System of sending packets with the lack of transmission control. Sometimes packets can be lost or unreceived, so sending 20 tick per seconds make the loss of packets a little less concerning.

# Alternatives

We considered using an Enum to define every type of component of the ECS so that it makes it possible to just send every "sparse_array" of the ECS Architecture. The problem with this method is that it would not have been possible to use the Boost.Serialization library to serialize our packets, which in return would have made some security issues over the sent and received data, that would be vulnerable to buffer overflows or injection attacks.

# References

Boost.Asio Library: https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio.html
Boost.Serialization: https://www.boost.org/doc/libs/1_83_0/libs/serialization/doc/tutorial.html
