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

let updateTitleWithNotifications = () =>
  Js.Global.setTimeout(
    () =>
      (
        switch (
          Storage.getConfig("github_notifications") |> int_of_string,
          Storage.getConfig("trello_notifications") |> int_of_string,
        ) {
        | (0, 0) => "Dashboard"
        | (g, t) => "Dashboard (" ++ (g + t |> string_of_int) ++ ")"
        }
      )
      |> updateTitle,
    500,
  );
