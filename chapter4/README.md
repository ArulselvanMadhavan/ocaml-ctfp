# Kleisli Categories
### Writer in Haskell
```ocaml
type 'a writer = 'a * string
```
* Morphisms from an arbitrary type to Writer type
```OCaml
'a -> 'b writer
```
* Kleisli for Writer
```ocaml
module type Kleisli =
  sig
    type a
    type b
    type c
    val ( >=> ) : (a -> b writer) -> (b -> c writer) -> a -> c writer
  end
```
* Pure for Writer
```ocaml
# let pure x = (x, "");;
val pure : 'a -> 'a * string = <fun>
```
* upCase for Writer
```ocaml
# let up_case : (string -> string writer) = fun s -> (String.uppercase s, "up_case ")
val up_case : string -> string writer = <fun>
```
* toWords for Writer
```ocaml
# let to_words : (string -> string list writer) = fun s -> (String.split s ~on:' ', "to_words ")
val to_words : string -> string list writer = <fun>
```
* Example Kleisli application
```ocaml
module KleisiExample(K : Kleisli with type a = string and type b = string and type c = string list) = struct
  let up_case_and_to_words : string -> string list writer = K.(>=>) up_case to_words
end
```
