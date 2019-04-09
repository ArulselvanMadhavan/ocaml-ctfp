# Categories Great and Small
## No Objects
    * Categories with no objects
    * No morphisms
### Monoid
* Definition
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
