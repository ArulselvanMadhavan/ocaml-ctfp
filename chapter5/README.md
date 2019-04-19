# Products and Coproducts
* Universal Construction
  - Defining objects in terms of their relationships.
* Initial Object
  - The object that has one and only arrow to any object in the category.
  - This object is unique upto isomorphism.
* Absurd definition
```ocaml
# type void
```
```ocaml
# let rec absurd (x:void) = absurd x
```
* Terminal Object
  - One and only morphism coming to it from any object in the category.
  - Unique upto isomorphism.
```ocaml
# let unit x = ()
```
```ocaml
# let yes _ = true
```
```ocaml
# let no _ = false
```
* For any Category C, we can reverse the arrows and define opposite category.
* Constructions in the opposite category are prefixed with "co".
## Isomorphisms
* Propositional equality, intensional equality, extensional equality and equality as a path in homotopy type theory.
* Isomorphism is an even weaker notion of equivalence.
* Isomorphism - Object that look the same and have an one to one mapping between them(Invertible morphism)
```ocaml
# let compose f g x = f (g x)
# let id x = x
```
* Pseudo Ocaml for expression function equality
```ocaml
compose f g = id
```
* Initial and Terminal objects are *unique upto unique isomorphism*.
* This uniqueness upto unique isomorphism is the basis for all universal construction.
## Products
```ocaml
# let fst (a,b) = a
```
```ocaml
# let snd (a, b) = b
```
```ocaml
# let fst (a,_) = a
# let snd (_, b) = b
```
* Object *c* and two morphisms *p* and *q* connecting it to *a* and *b*
```ocaml
module type Chapter5_Product = sig
  type a
  type c
  type b
  val p : c -> a
  val q : c -> b
end
```
* Example with *Int* and *Bool*
```ocaml
module Chapter5_Product_Example: Chapter5_Product with type a = int and type b = bool and type c = int = struct
  type a = int
  type b = bool
  type c = int
  let p (x:int) = x
  let q (_:int) = true
end
```
* Example with *c* as (int, int, bool)
```ocaml
module Chapter5_Product_Example2: Chapter5_Product = struct
  type a = int
  type b = bool
  type c = int * int * bool
  let p (x,_,_) = x
  let q (_,_,b) = b
end
```
* P' and Q' from *p* and *q* using *m*
```ocaml
# let m (x:int) = x
# let p' = compose Chapter5_Product_Example.p m
# let q' = compose Chapter5_Product_Example.q m
```
* m as a function returning pair (int, bool)
```ocaml
# let m (x:int) = (x, true)
# let p x = fst (m x)
# let q x = snd (m x)
```
* With, m as a function taking (int, int, bool)
```ocaml
# let m ((x,_,b):int*int*bool) = (x, b)
```
* Pseudo OCaml showing function equivalence
```
fst = compose p m'
snd = compose q m'
```
* m' example
```ocaml
# let m' ((x, b): int * bool) = (x, x, b)
```
* m' another example
```ocaml
# let m' ((x, b): int * bool) = (x, 42, b)
```
* Projection example
```ocaml
module type Chapter5_product_projection_example = functor (Product : Chapter5_Product) ->
sig
  val m : Product.c -> Product.a * Product.b
end
```
```ocaml
module ProjectionImpl(Product:Chapter5_Product) = struct
  let m c = (Product.p c, Product.q c)
end
```
* factorizer example
```ocaml
module type Factorizer = functor (Product: Chapter5_Product) ->
sig
  val factorizer : (Product.c -> Product.a) -> (Product.c -> Product.b) -> (Product.c -> Product.a * Product.b)
end
```
```ocaml
module FactorizerImpl(Product:Chapter5_Product) = struct
  let factorizer ca cb = (Product.p ca, Product.q cb)
end
```
* CoProduct
```ocaml
module type CoProduct = sig
  type a
  type b
  type c
  val i : a -> c
  val j : b -> c
end
```
* Pseudo Ocaml showing function equivalence
```
i' == compose m i
j' == compose m j
```
* Coproduct is the disjoint union of two sets.
* example
```ocaml
type contact = 
  | PhoneNum of int
  | EmailAddr of string
```
* example function
```ocaml
# let helpdesk = PhoneNum 2222222
```
* Either type
```ocaml
type ('a, 'b) either =
  | Left of 'a
  | Right of 'b
```
* Factorizer
```ocaml
let factorizer i j = function
  | Left a -> i a
  | Right b -> j b
```
* Definition of terminal object can be obtained by reversing the arrows of an initial object.
* Definition of coproduct can be obtained by reversing the arrows of product
* Pseudo OCaml
```OCaml
p = compose fst m
q = compose snd m
```
```OCaml
p () = fst (m ())
q () = snd (m ())
```
* Functions are asymmetric. Should be defined for every element of its domain but doesn't have to cover the whole codomain.
* Functions that tightly fill their codomain are called *surjective* or *onto*.
* Functions are allowed to map many elements from the domain to a single element in the codomain. Collapsing functions are called *injective* or *one-to-one*
* Functions that are neither embedding nor collapsing called *bijections*. They are symmetric and invertible. Example: Isomorphic functions.
