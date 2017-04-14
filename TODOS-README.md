# A3-Project
CPSC 323 Lexer, Parser, etc.
TODOs:
Feel free to add items as things come up or refactoring/splitting is necessary.

**Try not to delete items, rather one should mark it as cancelled.**


# Legend & General Info
- [ ] Not started
  - TODO not started past initial documentation or stubs.
- [x] Finished
  - Code is complete, documented, free of known bugs.

- For issues, specify underneath.
  * Some sort of miscellaneous issue with the TODO, specify underneath as best you can
- Cancelled Items
  * If a task is cancelled, adding a reason will help remember why it was cancelled in the first place.



# Critically Necessary TODOs
These tasks must be done to consider being a working version. Try to make these short and document stubs in code to explain structure, etc. These are in no particular order.

- [ ] Symbol Class: Type, \*Rule, (Explicit Rule)
- [ ] Rule Class: LHS, RHS[array] //(CAPS NONT, lower term)
- [ ] Node Class: \*symbol, kids[], kidcnt
- [ ] SERIALIZE token list (pretty much done)
  - convert lexer output to serialized list for use
- [ ] Feed tokens to parser
- [ ] Token Class: Name, type
- [ ] LRE (avoid recursive infloop)
- [ ] Left factoring (pretty much done)
- [ ] Error marking



# Bug Fixes
List any bugs with priority number included:

- [x] - Bug squashed!
- [ ] Existing bug with priority and description
  * [3] High priority, detrimental to program function
  * [2] - Moderate priority, potentially affects program function in certain circumstances.
  * [1] - Low priority, usually visual things like capitalization/spacing errors, output formatting


## BUGS:
- [ ] [3] (example) First line of output has an extra space
  - extra detail about it


# Wishlist
- [ ] Example item
  - details about it
- [ ] Another item
