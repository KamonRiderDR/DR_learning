# **Stack && Queue**

## Definition of stack & queue

All are **limited** *Linear List*!



## Rat in a maze





## Infix $\leftrightarrow$ postfix





## Train in & out





**Analysis**:

For each arrangement $P_i = <x_j>$, for each value $x_j$, we cannot decide the elements before it. However, for all the elements $x_k$ after $x_j$, we have $x_k<x_j$. In other word, the `top()` of stack is the **largest** value in the stack! 

==<font color = red>序列中的任何一个数其后面所有比它小的数应该是倒序的!</font>==

Problem? $\Rightarrow$ We need too much memory!

------

How do we use <font color = red>***stack***</font> to realize it?

The answer can be in the form of a **tree**.$<x_1,x_2,...,x_n>$. $\Rightarrow$ ==Backtracking==!( How? )

We can realize it by simulating the process of a train.(<font size = 2> *We may ignore other details and regard it as a Black Box. Thus, just input and output are needed.* </font>)

- End state $\to$ print
- train out $\to$ pop $\to$ backtrack
- train in $\to$ push $\to$ backtrack

<div align = "center">When to push and When to pop?</div>

```c++
    //  case 2: pop -> backtracking
    if( !station.empty() )
    {
        int temp = station.top();
        station.pop();
        result.push_back( temp );
        trainBacktracking( arrayTrain,FNum,SNum-1 );
        station.push( temp );
        result.pop_back();        
    }

    //  case 3: push -> backtracking
    if( FNum<sizeofTrain )
    {
        int temp = arrayTrain[ FNum ];
        station.push( temp );
        trainBacktracking( arrayTrain,FNum+1,SNum+1 );
        station.pop();
    }
```

------

