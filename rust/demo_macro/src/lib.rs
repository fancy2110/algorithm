use proc_macro2::TokenStream;
use quote::{quote, quote_spanned};
use std::collections::HashSet as Set;
use syn::{
    self,
    punctuated::{Pair, Punctuated},
    spanned::Spanned,
    Data, Ident, MetaList, Variant,
};

#[proc_macro_derive(HelloMacro, attributes(name))]
pub fn hello_macro_derive(input: proc_macro::TokenStream) -> proc_macro::TokenStream {
    // Construct a representation of Rust code as a syntax tree
    // that we can manipulate
    let ast = syn::parse(input).unwrap();

    // Build the trait implementation
    let expanded = impl_hello_macro(&ast);

    // Hand the output tokens back to the compiler.
    proc_macro::TokenStream::from(expanded)
}

#[proc_macro_attribute]
pub fn show_streams(
    attr: proc_macro::TokenStream,
    item: proc_macro::TokenStream,
) -> proc_macro::TokenStream {
    println!("attr: \"{}\"", attr.to_string());
    println!("item: \"{}\"", item.to_string());
    item
}

fn impl_hello_macro(ast: &syn::DeriveInput) -> TokenStream {
    let name = &ast.ident;
    let attrs = &ast.attrs;

    println!("item: \"{}\"", name.to_string());

    let data = &ast.data;
    let sums = match *data {
        Data::Enum(ref data) => {
            let recurse = data.variants.iter().map(|item| {
                let item_name = &item.ident;
                let item_attr = &item.attrs;
                quote_spanned! { item.span() =>  #name::#item_name => "hello", }
            });

            quote! {
                #(#recurse)*
                _ => "empty"
            }
        }

        Data::Struct(_) | Data::Union(_) => unimplemented!(),
    };

    quote! {
        impl HelloMacro for #name {
            fn hello_macro() {
                println!("Hello, Macro! My name is {}!", stringify!(#name));
            }
        }

        impl ToString for #name {
            fn to_string(&self) -> String {

                match(&self) {
                    #sums
                }
            }
        }
    }
}
