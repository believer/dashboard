open Webapi.Dom;

let pageTitle = () =>
  switch (document |> Document.asHtmlDocument) {
  | Some(v) => v->HtmlDocument.title
  | None => ""
  };

let updateTitle = newTitle =>
  switch (document |> Document.asHtmlDocument) {
  | Some(v) => v->HtmlDocument.setTitle(newTitle)
  | None => ()
  };

let updateTitleWithNotifications = (github, trello) =>
  switch (github |> int_of_string, trello |> int_of_string) {
  | (0, 0) => "Dashboard" |> updateTitle
  | (g, t) => "Dashboard (" ++ (g + t |> string_of_int) ++ ")" |> updateTitle
  };
