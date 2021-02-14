## ==FSM== in string matching

Components of FSM:$(Q,q_0,A,\Sigma,\delta)$
$$
\begin{cases}
Q:all \ states
\\
q_0:initial \ state
\\
A:state \ that \ means \ accept
\\
\Sigma:input \ alphabet
\\
\delta: transfer \ function
\end{cases}
$$

**辅助函数**：$\sigma(x) = max\{k:P_k\sqsupset x\}$.
$$
\Rightarrow
\begin{cases}
suffix \ of \ x
\\
prefix \ of \ P_k
\end{cases}
$$
$\sqsupset$ means $P_k$ is the suffix of $x$.

Transition function:
$$
\delta(q,a) = \sigma(P_qa)
$$

$q$ is the number state. Different string may have the same $q$ value! 

Notice that the **string in the bracket is $P$!**

As to $\sigma(x)$,obviously we have:

- $\forall x,a,\sigma(xa) \le \sigma(x)+1$
- $\forall x,a,if \ q = \sigma(x),then\ \sigma(xa) = \sigma(P_q,a)$





### Process

We define the text **T**, and the pattern string **P**. Then $||Q|| = P.length()$

- Process of FSM:


<img src=".\image\FSM_01.png" style="zoom:80%;" />



- The calculation of $\delta(x)$:


<img src=".\image\FSM_02.png" style="zoom:80%;" />

​		$Analysis:$

1. The purpose is to find the max **k** such that $P_k \sqsupset P_qa$, the initial **k** is the rightmost place, we scan it from right to left.

2. Why do we set $k = \min(m+1,q+2)?$

   $m+1$ means we may accept it while $q+2$ actually means $q+1$ as we will do operation`k--`.

​		

```c++
//  judging if Pk is suffix of Pq-a
bool suffixMatchingPrefix( char*p,int k,int q,char a )
{
    if( k == 0 )
    return true;
    if( k == 1 )
    return p[0] == a;
    if( !strncmp( p,p+q-k+1,k-1 ) && p[k-1] == a )
    return true;
    else
    return false;

}

vector< map<char,int> > transitionFunction( char*pattern,const char* alphabet )
{
    int m = strlen( pattern );
    int k = 0;
    int j = 0;
    vector< map<char,int> > map( m+1 );
    for( int i = 0;i != m;i ++ )
    {
        // all the alphabet
        j = 0;
        while( alphabet[j] != '\0' )
        {   
            k = min( m+1,i+2 );
            //  repeat k--
            do
            {
                k--;
            }
            while( !suffixMatchingPrefix( pattern,k,i,alphabet[j] ) );// Pk is suffix of S
            map[ i ][alphabet[j]] = k;
            j++;
        }

    }
    return map;
}

void FSMMatching( char*text,char*pattern,vector<map<char,int>> map )
{
    int n = strlen( text );
    int m = strlen( pattern );
    int q = 0;
    for( int i = 0;i != n;i++ )
    {
        q = map[ q ][ text[i] ];
        if( q == m )
        cout<<"Pattern occurs with shift "<<i+1-m<<endl;
    }
}
```

