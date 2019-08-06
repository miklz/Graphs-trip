# Graphs-trip

This project is a prerequisite to conclude the "Programação II" class. This was a project of my choice, and the end goal is to finding routes between cities utilizing minimum spanning tree.

## Objective

The purpose of this project is to determine the path with the minimum cost relative to time that interconnect any given cities.

## The chose way

To accomplish the task it was implemented the Prim's Algorithm to generate the path with the lowest cost relative to the weight of the path.
Here is one demonstration of the Prim's Algorithm:

![Prime's Algorithm](https://upload.wikimedia.org/wikipedia/commons/9/9b/PrimAlgDemo.gif)

By Shiyu Ji - Own work, CC BY-SA 4.0, https://commons.wikimedia.org/w/index.php?curid=54420894

The Algorithm was coded in C and implemented using a chained list, it was used a table with the time between the major cities in my home country to puts the code to run.

## Results

The tests were consistent, we could foresee that the small cities were to surround the big cities. But some cities get isolated from the rest, due to the lack of time I couldn't fix the bug.

## Considerations

It would be more easily to implement the code if I would have chose the approach by matrix instead of chained lists 

