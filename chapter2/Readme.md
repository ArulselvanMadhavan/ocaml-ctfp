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
```
