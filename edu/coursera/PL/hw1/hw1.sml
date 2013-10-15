(* Home work 1
 * Author: Aman Orazaev
 *)


(* Returns true if first date is older than second. *)
fun is_older(first : (int*int*int), second: (int*int*int)) =
  if (#1 first) = (#1 second)
  then
    if (#2 first) = (#2 second)
    then (#3 first) < (#3 second)
    else (#2 first) < (#2 second)
  else (#1 first) < (#1 second)

(* Returns number of dates in the given month *)
fun number_in_month(dates: (int*int*int) list, month: int) =
  if null dates
  then 0
  else
    if (#2 (hd dates)) = month
    then 1 + number_in_month((tl dates), month)
    else number_in_month((tl dates), month)

(* Returns number of dates in the given months *)
fun number_in_months(dates: (int*int*int) list, months: int list) =
  if null months
  then 0
  else number_in_month(dates, (hd months)) + number_in_months(dates, (tl
  months))

(* Returns list of dates in the given month *)
fun dates_in_month(dates: (int*int*int) list, month: int) =
  if null dates
  then []
  else
    if (#2 (hd dates)) = month
    then (hd dates)::dates_in_month((tl dates), month)
    else dates_in_month((tl dates), month)

(* Returns list of dates in the given months *)
fun dates_in_months(dates: (int*int*int) list, months: int list) =
  if null months
  then []
  else dates_in_month(dates, (hd months)) @ dates_in_months(dates, (tl months))

(* Returns n-th element of given list of string *)
fun get_nth(strings: string list, n: int) =
  if n = 1
  then (hd strings)
  else get_nth((tl strings), n - 1)

(* Returns formatted string with given date *)
fun date_to_string(date: (int*int*int)) =
  get_nth(["January ", "February ", "March ",
           "April ", "May ", "June ", "July ",
           "August ", "September ", "October ",
           "November ", "December "], (#2 date)
          ) ^ Int.toString((#3 date)) ^ ", " ^ Int.toString((#1 date))

(* Returns index n of given list, where sum of first of n
 * digits are less than sum, but sum of n+1 digits is
 * greater or equal to sum.
 *)
fun number_before_reaching_sum(sum: int, digits: int list) =
  let
    fun head_accumulate(digits: int list) =
      ((hd digits) + (hd (tl digits)))::(tl (tl digits))

    fun number_before_reaching_sum_(sum: int, digits: int list, index: int) =
      if (hd digits) < sum
      then number_before_reaching_sum_(sum, head_accumulate(digits), index + 1)
      else index - 1
  in
    Int.max(0, number_before_reaching_sum_(sum, 0::digits, 0))
  end

(* Returns number of month corresponding to given day of year *)
fun what_month(day: int) =
  1 + number_before_reaching_sum(day, [31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31])

(* Returns list of months corresponding to range of given days *)
fun month_range(start_day: int, end_day: int) =
  if end_day < start_day
  then []
  else what_month(start_day)::month_range(start_day + 1, end_day)

(* Returns oldest date from given list of dates *)
fun oldest(dates: (int*int*int) list) =
  if null dates
  then NONE
  else
    if null (tl dates)
    then SOME (hd dates)
    else
      let
        val tail_oldest = oldest((tl dates))
      in
        if is_older((hd dates), valOf tail_oldest)
        then SOME (hd dates)
        else tail_oldest
      end

