# Types and Functions
* Declare a variable and assing a type
```ocaml
module type Chapter2_DeclareVariable = sig
  val x : int
end
```
* Declare a function and assign a type
```ocaml
module type Chapter2_DeclareFunction = sig
  val f : bool -> bool
end
```
* OCaml doesn't have null. Throw exceptions to introduce runtime errors
```ocaml
module Chapter2_Bottom : sig
  val f : bool -> bool
end = struct
  let f b = failwith "Not Implemented"
end
```
* Bottom is also a member of bool -> bool
```ocaml
module Chapter2_Bottom : sig
  val f : bool -> bool
end = struct
  let f = fun _ -> failwith "Not implemented"
end
```
* Functions that return bottom are called /Partial/
* /Hask/ can be treated as /Set/
* Factorial Function in OCaml
```ocaml
# open Core
# let fact n =
  List.fold (List.range 1 n) ~init:1 ~f:( * )
val fact : int -> int = <fun>
```
* Absurd in OCaml
```ocaml
type void
```
```ocaml
# let rec absurd (x:void) = absurd x;;
val absurd : void -> 'a = <fun>
```
* Function taking unit and returning any type
```ocaml
# let f44 ():int = 44
val f44 : unit -> int = <fun>
```
* fInt
```ocaml
# let fInt (x:int):unit = ()
val fInt : int -> unit = <fun>
```
* fInt Generic param
```ocaml
# let fInt (_:int):unit = ()
val fInt : int -> unit = <fun>
```
* A generic/polymorphic unit function
```ocaml
# let unit (type a) (_:a) = ()
val unit : 'a -> unit = <fun>
```
* Bool as ADT
```ocaml
type bool = false | true
```
