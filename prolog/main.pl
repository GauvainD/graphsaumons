:- foreign(node(?positive),[choice_size(1)]).
:- foreign(edge(?positive, ?positive),[choice_size(2)]).
:- foreign(commonNeighbor(+positive, +positive, ?positive), [choice_size(1)]).
:- foreign(properNeighbor(+positive, +positive, ?positive), [choice_size(1)]).

huv(U,V,HUV):-
    findall(X,commonNeighbor(U,V,X),C),
    length(C,HUV).

thisisatest(E):-
    findall(V,edge(1,V),E),
    write(E).

%maxhuv(HUV):-
    %node(U),
    %node(V),
    %\+(edge(U,V)),
    %huv(U,V,HUV),
    %\+((node(X),node(Y),\+(edge(U,V)),huv(X,Y,H),H > HUV)),
    %!.
