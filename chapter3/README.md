# Categories Great and Small
## No Objects
    * Categories with no objects
    * No morphisms
### Monoid
* Definition
  1. Requires the operation to be associative.
  2. There must be a special element that behaves like a unit.
```ocaml
module type Monoid = sig
  type a
  val mempty : a
  val mappend : a -> a -> a
end
```
* String Instance of Monoid
```ocaml
module StringMonoid:Monoid = struct
  type a = string
  let mempty = ""
  let mappend = (^)
end
```
* Function equality without specifying its arguments is described as "point-free".
### Monoid as Category
* Monoid is a single object category.
