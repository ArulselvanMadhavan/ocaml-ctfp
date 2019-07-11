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
module Chapter2_Bottom : Chapter2_DeclareFunction =
struct
  let f (b:bool):bool = failwith "Not Implemented"
end
```
* Bottom is also a member of bool -> bool
```ocaml
module Chapter2_Bottom : Chapter2_DeclareFunction =
struct
  let f : bool -> bool = fun _ -> failwith "Not implemented"
end
```
* Functions that return bottom are called /Partial/
* /Hask/ can be treated as /Set/
* Factorial Function in OCaml
```ocaml
# let fact n =
  List.fold (List.range 1 n) ~init:1 ~f:( * )
val fact : int -> int = <fun>
```
* Absurd in OCaml
```ocaml
type void
let rec absurd (x:void) = absurd x
```
* Function taking unit and returning any type
```ocaml
# let f44 () : int = 44
val f44 : unit -> int = <fun>
```
* f\_int
```ocaml
# let f_int (x:int) = ()
val f_int : int -> unit = <fun>
```
* f\_int Generic param
```ocaml
# let f_int (_:int) = ()
val f_int : int -> unit = <fun>
```
* A generic/polymorphic unit function
```ocaml
# let unit _ = ()
val unit : 'a -> unit = <fun>
```
* Bool as ADT
```ocaml
type bool = false | true
```
