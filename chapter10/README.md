# Natural Transformations
### Utilities used by the code below
```ocaml
type ('a, 'b) const = Const of 'a
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end
let (<.>) f g x = f (g x)
```
- Functor is a mapping between categories that preserve their structure.
- Embeds source category in target category by using the source as the blueprint.
- Functor 
  - may collapse the whole source category into one object in the target category.
  - map every object to a different object
  - map every morphism to different morphism
- Natural transformation help us compare these realizations.
  - Mappings of functors.
- Natural isomorphism is defined as a natural transformation whose components are all isomorphisms.
### Polymorphic Functions
- Endofunctors - Type constructors that map types to types. Source and target category are the same.
```OCaml
val alpha : 'a . 'a f -> 'a g
```
- Without universal quantification.
```OCaml
val alpha : 'a f -> 'a g
```
- Free theorems are the naturality conditions, in the case of natural transformations expressed using parametric polymorphism.
```ocaml
# let safe_head = function | [] -> None | x :: xs -> Some x
```
- Verifying the Naturality condition(Pseudo OCaml, since function equality cannot be expressed)
```OCaml
let fmap f <.> safe_head = safe_head <.> fmap f
```
- Cases to handle
```OCaml
(* Starting with empty list *)
let fmap f (safe_head []) = fmap f None = None
```
```OCaml
let safe_head (fmap f []) = safe_head [] = None
```
- Non empty list
```OCaml
let fmap f (safe_head (x :: xs)) = fmap f (Some x)= Some (f x)
```
```OCaml
let safe_head (fmap f (x :: xs)) = safe_head (f x :: f xs) = Some (f x)
```
- Implemenation of fmap for lists
```ocaml
# let rec fmap f = function | [] -> [] | (x :: xs) -> f x :: fmap f xs
```
- Implementation of fmap for option type
```ocaml
# let rec fmap f = function 
    | None -> None 
    | Some -> Some (f x)
```
- Natural transformation to or from the *Const* functor looks just like a function that's polymorphic in either the return type or argument type.
```ocaml
# let rec length : 'a list -> (int, 'a) const = function 
    | [] -> Const 0 
    | (_ :: xs) -> Const (1 + un_const (length xs)) and 
un_const : 'a 'c. ('c, 'a) const -> 'c = function 
    | Const x -> x
```
- In practice, length is defined as
```OCaml
val length : 'a list -> int
```
- Parametrically polymorphic function from `Const` functor
```ocaml
# let scam : 'a. ('int, 'a) const -> 'a option = function 
    | Const a -> None
```
- Reader type
```ocaml
type ('e, 'a) reader = Reader of ('e -> 'a)
```
- Functor instance for Reader type
```ocaml
module Reader_Functor(T : sig type e end) : Functor = struct
  type 'a t = (T.e, 'a) reader
  let fmap : 'a 'b. ('a -> 'b) -> 'a t -> 'b t = fun f -> function
    | Reader r -> Reader (fun e -> f (r e))
end
```
- Natural Transformation from Reader () a -> Maybe a
```OCaml
val alpha : (unit, 'a) reader -> 'a option
```
- dumb version
```ocaml
# let dumb : 'a. (unit, 'a) reader -> 'a option = function
    | Reader _ -> None
```
- obvious implementation
```ocaml
# let obvious : 'a. (unit, 'a) reader -> 'a option = function
    | Reader f -> Some (f ())
```

