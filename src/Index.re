open Webapi.Dom;

switch (document |> Document.asHtmlDocument) {
| Some(v) => v->HtmlDocument.setTitle("test")
| None => ()
};

ReactDOMRe.renderToElementWithId(<Router />, "root");
ReasonReact.Router.push("");