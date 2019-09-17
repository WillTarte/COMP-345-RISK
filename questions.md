# QUESTIONS FOR TEACHER

## General

 . Which version of the game are we playing ?

## Card / Deck

 . What to do if the number of countries is not divisible by three ?

## Map / MapLoader

#### Questions

 . In the example .map files, countries and continents have colour and x,y coordinates specified. Should we expect and ignore those values
    or not expect them and return an error if they are there ?
 . While handling errors, is a file that is not exactly to spec rejected or should it be used if it can (for example, extra information on a line, should it be ignored or be considered a critical error) ?
 . In the handout it says that a country is owned by a player and that it contains a number of armies, but in the .map files it only specifies the number of armies for the continents.

#### Assumptions made

 . The [file] section and any other unrecognized sections are simply ignored
 . Recognized and parsed sections are : [continents], [countries] and [borders]
 . A valid [continents] specification line has a name and a number of army. Any more information will trigger an error
 . A valid [countries] specification line has an id, a name and a continent. Any more information will trigger an error
 . A valid [borders] specification line has a country id and a list of neighbours (unrestricted).
 . No test for "each country must belong to one and only one continent" was written in the MapDriver.cpp file since it is performed in the MapLoader.cpp file
 . If the map is a connected graph, then the subgraphs also are connected.
