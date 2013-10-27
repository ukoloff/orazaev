(* Programming languages. Home work 2.
 *
 * Author: Aman Orazaev
 *)

(* if you use this function to compare two strings (returns true if the same
   string), then you avoid several of the functions in problem 1 having
   polymorphic types that may be confusing *)
fun same_string(s1 : string, s2 : string) =
    s1 = s2


(* Problem 1.a *)
fun all_except_option (str : string, strings : string list) =
let
  fun is_in_list(li : string list) =
    case li of [] => false
       | x::tail => same_string(x, str) orelse is_in_list(tail)

  fun all_except(li : string list) =
    case li of [] => []
       | x::tail => if same_string(x, str)
                    then all_except(tail)
                    else x::all_except(tail)
in
  if is_in_list(strings)
  then SOME (all_except strings)
  else NONE
end


(* Problem 1.b *)
fun get_substitutions1(lists : string list list, name : string) =
  case lists of
       [] => []
     | li::tail => case all_except_option(name, li) of
                        NONE => get_substitutions1(tail, name)
                      | SOME x => x @ get_substitutions1(tail, name)


(* Problem 1.c *)
fun get_substitutions2(lists : string list list, name : string) =
let
  fun aux(li : string list list, ans : string list) =
    case li of
         [] => ans
       | x::tail => case all_except_option(name, x) of
                         NONE => aux(tail, ans)
                       | SOME x => aux(tail, ans @ x)
in
  aux(lists, [])
end


(* Problem 1.d *)
fun similar_names(lists: string list list, full_name) =
let
  fun all_names(li, m, l) =
    case li of
         [] => []
       | x::tail => {first=x, middle=m, last=l}::all_names(tail, m, l)
in
  case full_name of {first=name, middle=m, last=l} =>
       full_name::(all_names(get_substitutions2(lists, name), m, l))
end




datatype suit = Clubs | Diamonds | Hearts | Spades
datatype rank = Jack | Queen | King | Ace | Num of int
type card = suit * rank

datatype color = Red | Black
datatype move = Discard of card | Draw

exception IllegalMove


(* Problem 2.a *)
fun card_color(s : suit, r : rank) =
  case s of
       Clubs => Black
     | Spades => Black
     | _ => Red


(* Problem 2.b *)
fun card_value(s : suit, r : rank) =
  case r of
       Num x => x
     | Ace => 11
     | _ => 10


(* Problem 2.c *)
fun remove_card(cs : card list, c : card, e : exn) =
  case cs of
       x::tail => if x = c
                  then tail
                  else x::remove_card(tail, c, e)
     | [] => raise e


(* Problem 2.d *)
fun all_same_color(cs : card list) =
  case cs of
       x::(y::tail) => card_color(x) = card_color(y) andalso all_same_color(y::tail)
     | _ => true


(* Problem 2.e *)
fun sum_cards(cs : card list) =
let
  fun aux(l : card list, sum : int) =
    case l of
         [] => sum
       | c::tail => aux(tail, card_value(c) + sum)
in
  aux(cs, 0)
end


(* Problem 2.f *)
fun score(cs : card list, goal : int) =
let
  val csum = sum_cards(cs)
  val divisior = if all_same_color(cs) then 2 else 1
in
  if csum > goal
  then 3 * (csum - goal) div divisior
  else (goal - csum) div divisior
end


(* Problem 2.g *)
fun officiate(cs : card list, ms : move list, goal : int) =
let
  fun play(cl : card list, hl : card list, ml : move list) =
    case ml of
         m::mtail => (case m of
                          Discard c => play(cl, remove_card(hl, c, IllegalMove), mtail)
                        | Draw => (case cl of
                                        c::tail => play(tail, c::hl, mtail)
                                      | [] => score(hl, goal)))
       | [] => score(hl, goal)
in
  play(cs, [], ms)
end
