`ReadCSVFile`
Line 16: `NumEntries` returns an unsigned int; `num_expected_students` should either be an unsigned int (preferred) or `NumEntries(file_name)` should be cast to an int.
Line 19: The conditional should be `!ifs.is_open()` instead of `ifs.is_open()`, otherwise the program will throw an error when the file is successfully opened.

`ReadStudentRecFromStream`
Line 47: An `<<` operator is used when `>>` should be.
Line 52: `throwaway` is called instead of `throw_away`.
Line 57: The comma is not removed from `last_name`.
Line 60: The function is missing a return statement.

`FilterStudents`
Line 87: The for loop uses the size of `filtered`, an empty vector, when looping through `students`.
Line 88: `filtered` has not been initialized with any values, so any attempts to access `filtered` at any index well return an error. The if body should be replaced with `filtered.push_back(students[i])`.

`GetAverage`
Line 98: Missing a closing parenthesis
Line 101: Vectors are zero-indexed, but the quiz numbers are not. The code should access the score at index `quiz_no - 1`.

`FindOutliers` 
Line 35: `quiz_no` is never validated, so `quiz_no - 1` can underflow if `quiz_no == 0`.
Line 61: outlier check uses `<=` / `>=`, so boundary values get counted as outliers.

`WriteGroupSummary`
Line 84: each grade is cast to `int` before summing, so decimal precision is lost.
Line 87: average is computed with integer division.

`CalculateGrade`
Line 124: empty input returns `0.0` instead of throwing.
Lines 127-129: catch-up is applied in reverse order, so we cascade back
Lines 131-134: accumulation uses `int`, this removes decimal quiz scores.
Line 136: final division is integer division.

`WriteGradeStatistics`
Lines 151-155: CS-major and non-CS-major groups are swapped.

`WriteGradeReport` 
Lines 172-174: loop uses `<=` and reads one past the end of `quiz_scores`.
