## Top-Down Parser

 1. BackTracking
    - Recursive Descent Parsing 
 2. Non BackTracking
    - Recursive Parser
    - Predicative Parser
    - LL(1)
    - Error Recovery (Panic-mode recovery)

Points:
 - For top down parsing we need to remove left recursion and left factoring.
 - A Grammar G is LL(1) if and only if whenever S - C | D are two distinct productions of G the following conditions hold:
    - For no terminal 'a' do both C and D derive strings beginning with a.
    - At most one of C and D can derive the empty string.
    - If D =*=> epsilon, then C does not derive any string beginning with 'a' terminal in FOLLOW (C).