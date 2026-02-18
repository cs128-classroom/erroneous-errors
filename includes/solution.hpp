#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <cstddef>
#include <iosfwd>
#include <stdexcept>
#include <string>
#include <vector>

#include "student.hpp"

std::vector<Student> ReadCSVFile(const std::string& file_name);
Student ReadStudentRecFromStream(std::istream& ifs);
unsigned int NumEntries(const std::string& file_name);
std::vector<Student> FilterStudents(const std::vector<Student>& students,
                                    bool is_cs_major);
double GetAverage(const std::vector<Student>& students, unsigned int quiz_no);
std::vector<double> FindOutliers(const std::vector<Student>& students,
                                 unsigned int quiz_no);
double CalculateGrade(std::vector<double> quiz_scores);
double ApplyCatchup(double prev_quiz, double curr_quiz);
double Median(const std::vector<double>& sorted_values,
              std::size_t begin,
              std::size_t end);
void WriteGroupSummary(std::ofstream& output,
                       const std::string& group_name,
                       const std::vector<Student>& group);
void WriteGradeStatistics(const std::string& file_name,
                          const std::vector<Student>& students);
void WriteGradeReport(const std::string& file_name, Student subject);

#endif  // SOLUTION_HPP
