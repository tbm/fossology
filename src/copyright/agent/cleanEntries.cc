/*
 * Copyright (C) 2014, Siemens AG
 * Author: Johannes Najjar
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "cleanEntries.hpp"

/* Trims a string of any characters provided in the char list */
std::string trim(std::string str, std::string charlist = " \t\f\v\n\r")
{
size_t last;
last = str.find_last_not_of(charlist);
// only contains chars that are being trimmed, return ""
if (last == std::string::npos)
{
  return "";
}

size_t first = str.find_first_not_of(charlist);
if (first == std::string::npos)
{
  first = 0;
}

return str.substr(first, (last-first)+1);
}

/**
 *
 * @return true if entry needs to be written to database
 */
bool CleanDatabaseEntry(DatabaseEntry& input) {

  std::string newtext = " ";

  input.content = rx::regex_replace(input.content, rx::regex("[\\x0-\\x1f]"), newtext);

    //This is ugly, we should not use strings, neither here nor in the database to distinguish types
  if (input.type.compare("statement") == 0 )
  {
    /* !"#$%&' */
    input.content = rx::regex_replace(input.content, rx::regex("([\\x21-\\x27])|([*@])"), newtext);
    /*  numbers-numbers, two or more digits, ', ' */

//TODO    input.content = rx::regex_replace(input.content, rx::regex("(([0-9]+)-([0-9]+))|([0-9]{2,})|(,)"), newtext);
//TODO    input.content = rx::regex_replace(input.content, rx::regex(" : "), newtext);// free :, probably followed a date
  }
  else
  if (input.type.compare("email") == 0 )
  {
    //$content = str_replace(":;<=>()", " ", $content);
    // I do not understand the above, I would assume they want to replace any of the characters with space
    // but the function replaces the sequence ...
    // This is a slow variant that does that. We need some | if we want to replace all of them
    // TODO
    input.content = rx::regex_replace(input.content, rx::regex(":;<=>()"), newtext);

  }

  /* remove double spaces */
  input.content = rx::regex_replace(input.content, rx::regex("\\s\\s+"), newtext);
  /* remove leading/trailing whitespace and some punctuation */
  input.content = trim(input.content, "\t \n\r<>./\"\'");


  /* remove leading "dnl " */
  if ((strlen(input.content.c_str()) > 4) &&
  (input.content.compare(0, 4,"dnl ") == 0))
    input.content = input.content.substr(4);

  /* skip empty content */
  if (input.content.size()==0) return false;

  /* Step 1B: rearrange copyright statments to try and put the holder first,
   * followed by the rest of the statement, less copyright years.
  */
  /* Not yet implemented
   if ($row['type'] == 'statement') $content = $this->StmtReorder($content);
  */

  //This has to be calculated from the database
  //$row['copyright_count'] = 1;

  //TODO find a nicer way to write this...
//  $row['hash'] = md5($row['content']);

  return true;

}
