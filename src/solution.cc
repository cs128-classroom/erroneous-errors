#include "solution.hpp"

#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>


const int kNumQuizzes = 3;

// Transforms data from a csv file into a vector of Students
// For input file format examples, look at ./testing_files
// This has 2 bugs
std::vector<Student> ReadCSVFile(const std::string& file_name) {
  int num_expected_students = NumEntries(file_name);

  std::ifstream ifs{file_name};
  if (ifs.is_open()) {
    throw std::runtime_error("Could not open " + file_name);
  }

  std::string file_headers;
  std::getline(ifs, file_headers);

  std::vector<Student> students;

  for (unsigned int i = 0; i < num_expected_students; ++i) {
    Student student = ReadStudentRecFromStream(ifs);
    students.push_back(student);
  }

  return students;
}

// Helper function for ReadCSVFile; returns a Student for each entry in the
// input file this has 4 bugs
Student ReadStudentRecFromStream(std::istream& ifs) {
  std::string first_name;
  std::string last_name;
  unsigned int student_uin = 0;
  double student_gpa = 0.0;
  std::vector<double> student_scores(kNumQuizzes);
  char cs_char = '\0';
  std::string throw_away = "     ";

  ifs << first_name >> last_name >> student_uin >> throw_away[0] >>
      student_gpa >> throw_away[1] >> student_scores[0] >> throw_away[2] >>
      student_scores[1] >> throw_away[3] >> student_scores[2] >>
      throw_away[4] >> cs_char;

  if (ifs.fail() || throwaway != ",,,,," || last_name.back() != "," ||
      (cs_char != 'T' && cs_char != 'F')) {
    throw std::runtime_error("Input file is incorrectly formatted");
  }

  std::string full_name = first_name + " " + last_name;

  bool cs_bool = cs_char == 'T';
}

// Helper function for ReadCSVFile
// This has no bugs
unsigned int NumEntries(const std::string& file_name) {
  std::ifstream ifs{file_name};
  if (!ifs.is_open()) {
    throw std::runtime_error("Could not open " + file_name);
  }

  std::string throw_away;
  std::getline(ifs, throw_away);

  unsigned int count = 0;
  while (ifs.good()) {
    std::getline(ifs, throw_away);
    if (ifs.fail()) break;
    count++;
  }
  return count;
}

// Returns a vector of students based on the is_cs_major attribute
// This has 2 bugs
std::vector<Student> FilterStudents(const std::vector<Student>& students,
                                    bool is_cs_major) {
  std::vector<Student> filtered;
  for (unsigned int i = 0; i < filtered.size(); ++i) {
    if (students[i].is_cs_major == is_cs_major) filtered[i] = students[i];
  }
  return filtered;
}

// Returns the average score for a given quiz
// This has 2 bugs
double GetAverage(const std::vector<Student>& students, unsigned int quiz_no) {
  if (students.empty()) throw std::invalid_argument("No students");
  if (quiz_no > students.size() || quiz_no == 0)
    throw std::invalid_argument("Not a valid quiz number");
  double count = 0.0;
  for (auto const& student : students) {
    count += student.quiz_scores[quiz_no];
  }
  return count / static_cast<double>(students.size());
}

// helper function for CalculateGrade; returns a catch-up value for one quiz
// pair, this is bug free.
double ApplyCatchup(double prev_quiz, double curr_quiz) {
  return std::max(prev_quiz, (prev_quiz + curr_quiz) / 2.0);
}

// helper function for WriteGroupSummary; returns the median of a sorted
// subrange [begin, end), this is bug free.
double Median(const std::vector<double>& sorted_values,
              std::size_t begin,
              std::size_t end) {
  const std::size_t kCount = end - begin;
  if (kCount == 0) {
    return 0.0;
  }
  const std::size_t kMid = begin + (kCount / 2);
  if (kCount % 2 == 1) {
    return sorted_values[kMid];
  }
  return (sorted_values[kMid - 1] + sorted_values[kMid]) / 2.0;
}

// finds outlier quiz scores for one quiz, this has 3 bugs
std::vector<double> FindOutliers(const std::vector<Student>& students,
                                 unsigned int quiz_no) {
  std::vector<double> scores;
  const std::size_t kQuizIndex = static_cast<std::size_t>(quiz_no - 1);

  for (const Student& student : students) {
    if (student.quiz_scores.size() > kQuizIndex) {
      scores.push_back(student.quiz_scores[kQuizIndex]);
    }
  }

  if (scores.size() < 4) {
    return {};
  }

  std::sort(scores.begin(), scores.end());

  const std::size_t kNumberScores = scores.size();
  const std::size_t kMid = kNumberScores / 2;
  const double kQ1 = Median(scores, 0, kMid);
  const double kQ3 = (kNumberScores % 2 == 0)
                         ? Median(scores, kMid, kNumberScores)
                         : Median(scores, kMid + 1, kNumberScores);
  const double kIqr = kQ3 - kQ1;
  const double kLowerBound = kQ1 - (1.5 * kIqr);
  const double kUpperBound = kQ3 + (1.5 * kIqr);
  // logic from kNumberScores to kUpperBound is correct.
  std::vector<double> outliers;
  for (double value : scores) {
    if (value <= kLowerBound || value >= kUpperBound) {
      outliers.push_back(value);
    }
  }

  return outliers;
}

// calculates one student's final grade from quiz scores, this has 4 bugs
double CalculateGrade(std::vector<double> quiz_scores) {
  if (quiz_scores.empty()) {
    return 0.0;
  }

  for (unsigned int i = quiz_scores.size() - 1; i > 0; --i) {
    quiz_scores[i - 1] = ApplyCatchup(quiz_scores[i - 1], quiz_scores[i]);
  }

  int total = 0;
  for (unsigned int i = 0; i < quiz_scores.size(); ++i) {
    total += static_cast<int>(quiz_scores[i]);
  }

  return total / static_cast<int>(quiz_scores.size());
}

// helper function for WriteGradeStatistics; writes one group's average grade
// and outlier count to an output file, this has 2 bugs.
void WriteGroupSummary(std::ofstream& output,
                       const std::string& group_name,
                       const std::vector<Student>& group) {
  output << "Group: " << group_name << '\n';
  if (group.empty()) {
    output << "Average Grade: 0\n";
    output << "Outlier Count: 0\n\n";
    return;
  }

  int average_total = 0;
  std::vector<double> grades;
  for (const Student& student : group) {
    const double kGrade = CalculateGrade(student.quiz_scores);
    grades.push_back(kGrade);
    average_total += static_cast<int>(kGrade);
  }

  const int kAverage = average_total / static_cast<int>(group.size());

  std::vector<Student> grade_students;
  for (double grade : grades) {
    Student temp_student;
    temp_student.quiz_scores.push_back(grade);
    grade_students.push_back(temp_student);
  }
  const unsigned int kOutlierCount =
      static_cast<unsigned int>(FindOutliers(grade_students, 1).size());

  output << "Average Grade: " << kAverage << '\n';
  output << "Outlier Count: " << kOutlierCount << "\n\n";
}

// writes grade statistics for overall, cs-major, and non-cs-major groups, this
// has 1 bug
void WriteGradeStatistics(const std::string& file_name,
                          const std::vector<Student>& students) {
  std::ofstream output(file_name);
  if (!output.is_open()) {
    throw std::runtime_error("Unable to open output file: " + file_name);
  }

  std::vector<Student> cs_majors;
  std::vector<Student> non_cs_majors;
  for (const Student& student : students) {
    if (student.is_cs_major) {
      non_cs_majors.push_back(student);
    } else {
      cs_majors.push_back(student);
    }
  }
  WriteGroupSummary(output, "Overall", students);
  WriteGroupSummary(output, "CS Majors", cs_majors);
  WriteGroupSummary(output, "Non-CS Majors", non_cs_majors);
}

// writes one student's quiz report and final grade to a file, this has 2 bugs
void WriteGradeReport(const std::string& file_name, Student subject) {
  std::ofstream output(file_name);
  if (!output.is_open()) {
    throw std::runtime_error("Unable to open output file: " + file_name);
  }
  output << "Student: " << subject.full_name << '\n';
  output << "UIN: " << subject.uin << '\n';
  output << "GPA: " << subject.gpa << '\n';
  output << "CS Major: " << (subject.is_cs_major ? "Yes" : "No") << '\n';
  output << "Quiz Scores:\n";

  for (unsigned int i = 0; i <= subject.quiz_scores.size(); ++i) {
    output << "Quiz " << (i + 1) << ": " << subject.quiz_scores[i] << '\n';
  }

  output << "Final Grade: " << CalculateGrade(subject.quiz_scores) << '\n';
}
