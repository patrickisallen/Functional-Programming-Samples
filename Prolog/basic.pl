member(X, [X|_]).                       % base case
member(X, [_|Rest]) :- member(X, Rest). % recursive case

mylen([],0).
mylen([_|Xs], Len) :-
    mylen(Xs, Rest_len),
    Len is 1 + Rest_len.

addup([], 0).
addup([Head | Tail], Sum) :-
    addup(Tail, SumRest),
    Sum is SumRest + Head.


% Question 1 -  Implement makelist(N, X, Lst) 
makelist(0,_,[]).
makelist(N, X, [X|Tail]) :- 
    N > 0, 
    N1 is N - 1,
    makelist(N1, X, Tail).


% Question 2 - second_min(Lst, M)
second_min([], X) :- false.
second_min([_], X) :- false.
second_min(List, M) :-
    sort(List, [First, Second | Rest]),
    M is Second.


% Question 3 -  mynumlist(Lo, Hi, Result)
mynumlist(X,X, [X]).
mynumlist(Lo, Hi, [Lo | Tail]) :-
    Lo < Hi,
    Lo1 is Lo + 1,
    mynumlist(Lo1, Hi, Tail).

% Question 4 -  all_diff(Lst)
all_diff([]) :- true.
all_diff([Head | Tail]) :-
    \+ member(Head, Tail),
    all_diff(Tail).

% Question 5 -  negpos(L, Neg, NonNeg)
negpos([], [], []).
negpos(List, Neg, NonNeg) :-
    exclude(>(0), List, NonNeg),
    exclude(<(0), List, Neg).

% Question 6 - magic(L9, Result)
printrow(P, Q, R) :-
    write(' '), write(P), write(' '), write(Q),
    write(' '), write(R), write(' '), nl.

magic(List, Result) :- 
    length(List, Num),
    Num < 9,
    Num > 9.
magic([First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth], Result) :-
     permutation([First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eighth, Ninth], [A,B,C,D,E,F,G,H,I]),
     addup([A,B,C], Total1),
     addup([D,E,F], Total2),
     addup([G,H,I], Total3),

     addup([A,D,G], Total4),
     addup([B,E,H], Total5),
     addup([C,F,I], Total6),

    Total1 = Total2,
    Total2 = Total3,
    Total3 = Total4,
    Total4 = Total5,
    Total5 = Total6,

     printrow(A,B,C),
     printrow(D,E,F),
     printrow(G,H,I),
     Result = [A,B,C,D,E,F,G,H,I].





