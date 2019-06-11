# Natural Transformations
### Utilities used by the code below
```ocaml
type ('a, 'b) const = Const of 'a
module type Functor = sig 
  type 'a t 
  val fmap : ('a -> 'b) -> 'a t -> 'b t 
end
let (<.>) f g x = f (g x)
module type Contravariant = sig
  type 'a t
  val contramap : ('b -> 'a) -> 'a t -> 'b t
end
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
    | Reader r -> Reader (f <.> r)
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
### Beyond Naturality
- Parametrically polymorphic function between two functors is always a natural transformation.
- Function types are covariant in their return type.
- Function types are contravariant in their argument type.
- Contravariant example
```ocaml
type ('r, 'a) op = Op ('a -> 'r)
```
- Contravariant instance
```ocaml
module Op_Contravariant(T : sig type r end): Contravariant = struct
  type 'a t = (T.r, 'a) op
  let contramap : ('b -> 'a) -> 'a t -> 'b t = fun f -> function
    | Op g -> Op (g <.> f)
end
```
- predToStr
```ocaml
# let pred_to_str = function
    | Op f -> Op (fun x -> if f x then "T" else "F")
```
- Contravariant functors satisfy the opposite naturality condition.
```OCaml
contramap f <.> pred_to_str = pred_to_str <.> contramap f
```
- Op Bool contramap signature
```ocaml
# module Op_Bool = Op_Contravariant(struct type r = bool end)
# Op_Bool.contramap
```
- Type constructors that are neither covariant or contravariant.
```OCaml
'a -> 'a
```
```OCaml
('a -> 'a) -> 'a f
```
- Dinatural transformation is a generalization of the natural transformations.
### Functor Category
- There is one category of functors for each pair of categories, C and D.
- Objects in this category are functors from C to D.
- Morphisms = Natural transformations between C and D.
- Composition of natural transformations is associative.
- Functor category between C and D is Fun(C, D) or |C, D| or D^C
### Revisiting abstractions
- Category
  - objects and morphisms
- Cat - Higher order category that contains other categories as objects.
- Morphisms in Cat are functors.
- Hom-Set in Cat is a set of functors. Cat(C,D) - Set of functors between C and D categories.
- Functor Category |C,D| is *also* a set of functors between C and D.
- Functor Category is also a category. So it must be part of Cat.
- Cat is a full blown Cartesian Closed Category in which there is an exponential object D^C for any pair of categories. This exponential object is a category - Functor Category Fun(C, D)
### 2-Categories
- Cat - Higher order category with categories as objects and functors as morphisms.
- Hom-Set in Cat is a set of functors.
- Set of functors form a category.
- Functors are morphisms in Cat. Natural transformations are morphisms between morphisms.
- 2-category contains
  - objects - Categories
  - 1 morphisms - Functors between categories
  - 2 morphisms between morphisms - Natural transformation between functors.
