# Logik-Interpreter 3.0
-----------------------------
Information:
-----------------------------
- Solves logical systems of equations (example - https://kpolyakov.spb.ru/school/test10bu/21a.htm).
- The data is entered into a file "data.txt", output of matching combinations in "data_out.txt".
- The answer (number of matching combinations) is output to the console. 
Bonus outputs the number of variables.
-----------------------------
Syntax:
-----------------------------
1) "^" - conjunctive operation (AND)
2) "+" - disjunction operation (OR)
3) "-" - implication operation (->)
4) "=" - equivalence operation
5) "!" - negation operation
-----------------------------
Example of input data:
-----------------------------

(x1 - x2) ^ (x2 - x3) ^ (x3 - x4) = 1
(y1 - y2) ^ (y2 - y3) ^ (y3 - y4) = 1
(z1 - z2) ^ (z2 - z3) ^ (z3 - z4) = 1
x1 ^ y2 ^ z3 = 0

!(x1 = x2) ^ !(x1 = x3) ^ (x2 = x3) = 0
!(x3 = x4) ^ !(x3 = x5) ^ (x4 = x5) = 0
!(x5 = x6) ^ !(x5 = x7) ^ (x6 = x7) = 0
!(x7 = x8) ^ !(x7 = x9) ^ (x8 = x9) = 0

(x1 - x2) ^ (x3 - x4 - x5 - x6) = 0
(y1 - y2 - y3) ^ (y4 - y5 - y6) = 1
(x2 - y2) ^ (x3 - y3) = 1

----------------------------
Restriction:
----------------------------
The program solves equations with 3 variables: x, y, z, of which there can be 13 sets each (i.e. x1, x2...x13; y1, y2...y13; z1, z2...z13). It is done so because during the tests there was no system with more than 3 types of variables and more than 13 variables of 1 type each.
