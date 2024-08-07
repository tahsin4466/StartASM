1. [ ] # Examples
2. [ ] ```
3. [ ] comment "Let's set a constant 21 and ask the user for their age"
4. [ ] create integer 21 to r1
5. [ ] print "How long till you're 21? Let's find out!"
6. [ ] print newline
7. [ ] print "Enter your age: "
8. [ ] input integer to r2
9. [ ] print newline
10. [ ] compare r2 with r1
11. [ ] jump if equal to 'is21'
12. [ ] jump if greater to 'past21'
13. [ ] jump if less to 'not21'
14. [ ] stop
15. [ ] 
16. [ ] comment "Case - if the user is exactly 21"
17. [ ] label 'is21'
18. [ ] print "You're just barely 21! Yay!"
19. [ ] stop
20. [ ] 
21. [ ] comment "Case - if the user is past 21, we'll print their age back to them"
22. [ ] label 'past21'
23. [ ] print "Wow, you're "
24. [ ] output r2
25. [ ] print " and you're using this calculator? Scram!"
26. [ ] stop
27. [ ] 
28. [ ] comment "Case - if the user is yet to be 21, we'll output the number of years left"
29. [ ] label 'not21'
30. [ ] sub r1 with r2 to r3
31. [ ] print " *sigh* you still have "
32. [ ] output r3
33. [ ] print " years to go till you're 21."
34. [ ] stop
35. [ ] ```
36. [ ] The above program demonstrates numerous features of StartASM, including I/O, initializing values, arithmetic operations, and conditional jumps.
37. [ ] 
38. [ ] Here is a seperate example showing a way to input a string given individual character inputs. It terminates once the ~ is detected:
39. [ ] ```
40. [ ] comment "First let's get the input string by looping until ~ is detected"
41. [ ] comment "First create three memory addresses - our start index, our current index and our increment amount"
42. [ ] create memory m<0> to r0
43. [ ] create memory m<0> to r1
44. [ ] create memory m<1> to r2
45. [ ] comment "Next create our terminating character"
46. [ ] create character ~ to r3
47. [ ] 
48. [ ] comment "Prompt the user for input"
49. [ ] print "Enter your string until you terminate with the ~ key"
50. [ ] print newline
51. [ ] 
52. [ ] comment "Loop the input until we detect the ~ key"
53. [ ] label 'inputLoop'
54. [ ] input character to r4
55. [ ] compare r4 with r3
56. [ ] jump if equal to 'outputLoop'
57. [ ] store r4 to r1
58. [ ] add r1 with r2 to r1
59. [ ] jump if unconditional to 'inputLoop'
60. [ ] 
61. [ ] comment "Now let's print the string back to them starting from our start index until it hit's our 'current' (or end) index"
62. [ ] label 'outputLoop'
63. [ ] load r0 to r4
64. [ ] output r4
65. [ ] add r0 with r2 to r0
66. [ ] compare r0 with r1
67. [ ] jump if greater to 'terminateProgram'
68. [ ] jump if unconditional to 'outputLoop'
69. [ ] 
70. [ ] label 'terminateProgram'
71. [ ] stop
72. [ ] ```
73. [ ] 
74. [ ] The compiler will also return syntax and symbol/scope error messages that show the excepted line, token, and what the compiler expected, if applicable. For example, submitting this invalid code:
75. [ ] ``` 
76. [ ] shift right r2 with r1
77. [ ] junk r1
78. [ ] move m<1> to r6
79. [ ] ```
80. [ ] 
81. [ ] Will return the following errors:
82. [ ] ```
83. [ ] Invalid syntax at line 1: 'shift right r2 with r1'
84. [ ] Unknown conjunction 'with'. Expected 'by'
85. [ ] 
86. [ ] Invalid syntax at line 2: 'junk r1'
87. [ ] Unknown instruction 'junk'
88. [ ] 
89. [ ] Invalid syntax at line 3: 'move m<1> to r6'
90. [ ] Unknown source 'm<1>'. Expected register r0-r9
91. [ ] ```
92. [ ] 