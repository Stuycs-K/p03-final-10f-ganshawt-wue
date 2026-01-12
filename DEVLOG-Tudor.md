# Dev Log:

This document must be updated daily every time you finish a work session.

## Tudor Ganshaw

### 2025-01-06 - Question Creation System
Started work on the question creation system.
Implemented the generation questions (45m) and the height/weight questions (30m). Set up the system for future questions to be implemented as well.

## 2025-01-07 - Finishing question creation system.
Finished question creation system.
Implemented the type, stat, and base experience questions (60m). Also made it easier for the same questions and answers to be generated multiple times over.

## 2025-01-08 - Trying to fix bugs
Fixed question creation system to work with question asking system
Allowed server to send questions to the clients properly
Tried to fix issues with server randomly desyncing from clients or disconnecting (failed).

## 2025-01-09 - More bug fixes
Fixed issue with generation questions breaking due to an issue in questioncreation.c
Desync issue still persists

## 2025-01-11 - Even more bug fixes
Fixed issue where non-valid inputs would be accepted
Fixed part of one desync issue
Issues still persist with desync & with questions occasionally not printing correctly
