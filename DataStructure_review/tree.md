# Tree

## Definition





## Operation





## BST





## Winner/Loser Tree





## Heap





## Union-Find Set

<font size = 4>$\forall S_i,S_j,S_i \bigcap S_j = \varnothing$</font>

**Operations:**

- $S_i \cup S_j \to union$
- `Find(i)`: find the root of each node $i$

Notice that here children point to parents node!

<div align = "center">UNION(i,j)</div>

$$
parent[j] = i
$$

(j and i are both roots.)

<div align = "center">FIND(i)</div>

$$
while(parent[i] >= 0)
\\
i = parent[i]
\\
return \ i
$$



We can use array or linked list to realize it!

**ARRAY**

We set $parent[i] = i$ in the beginning.