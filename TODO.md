# TODO
## High Priority
- [ ] Get server connection working
- [ ] Nontrivial rule checks
    - [ ] Relative time scheduling (after_last and before_next_scheduled)
    - [x] Schedule cron expressions
    - [x] File change event listener
- [ ] Documentation
- [ ] Write tests
- [x] Override rule check
- [x] Load configs
- [x] Load rules
- [x] Basic rule checking

## Medium Priority
- [ ] Handle new directory creation inside target
- [ ] Pre-commit hook to standardize naming
- [x] Auto-generate configs if not detected, including example target definition
- [x] Fix logger to not generate a file until a crash occurs
- [x] Ignore paths on listener creation rather than on event
- [x] Add escaped characters to regex patterns instead of windows-style paths
- [x] Only check updated targets
- [x] Listener path ignores
- [x] Implement logger, remove print statements
- [x] Improve performance by waking threads instead of polling
- [x] Specify custom formatting in [.clang-format](.clang-format)

## Low Priority
- [ ] Remove cppcheck-suppress comments
- [ ] Improve command line argument documentation
- [x] Add `show w` and `show c` commands from GPLv3 license
