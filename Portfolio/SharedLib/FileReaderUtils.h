// FileReaderUtils.h
#pragma once
#if !defined(FILEREADERUTILS_H)
#define FILEREADERUTILS_H
#include <string>
/**
 * @brief [Internal] Safely parses the "records:=N" part of a header line.
 * @param aHeaderLine The line, e.g., "[NODES;records:=11]"
 * @return The number of records, or 0 if not found.
 */
int GetRecordCount(const std::string& aHeaderLine);

#endif // FILEREADERUTILS_H