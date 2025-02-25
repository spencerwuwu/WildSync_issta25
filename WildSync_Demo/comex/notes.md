## C-Sharp to CPP
- Cool and right function parameters
- Direct subsitution
    - c parameter -> parameter_declaration
    - block -> compound_statement
    - c local_declaration_statement -> declaration
    - invocation_expression -> call_expression
    - variable_declarator -> init_declarator
    - c member_access_expression -> field_expression
    - c element_access_expression -> subscript_expression

c
```
local_function_statement -> []
---
function_definition
```
c
```
variable_declaration -> [] 
---
declaration
```

```
if_statement -> [exp, block] 
--
if_statement -> [condition_clause -> exp, compound_statement]
```

```
assignment_expression -> [exp, assigment_operator, exp]
--
assignment_expression -> [exp, exp]
```

```
switch_statement -> 
[ exp, 
  switch_body -> [switch_section -> [
                    case_switch_label,
                    exp...
                    ]
                ]
]
---
switch_statement -> 
[ condition_clause, 
  compound_statement -> [ case_statement -> [
                                exp, 
                                exp...
                            ]
                        ]
]
```
