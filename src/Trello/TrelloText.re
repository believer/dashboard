let component = ReasonReact.statelessComponent("TrelloText");

let make = (~attachment, ~listAfter, ~listBefore, ~text, _children) => {
  ...component,
  render: _self =>
    <div className="mb2 lh-copy">
      {
        switch (listBefore, listAfter) {
        | (Some(before), Some(after)) =>
          before##name ++ " -> " ++ after##name |> Utils.str
        | (Some(_), None)
        | (None, Some(_))
        | (None, None) => ReasonReact.null
        }
      }
      {
        switch (text) {
        | Some(t) => t->Utils.str
        | None => ReasonReact.null
        }
      }
      {
        switch (attachment) {
        | Some(a) =>
          switch (a##attachmentType) {
          | "image" => <img src=a##url />
          | _ => ReasonReact.null
          }
        | _ => ReasonReact.null
        }
      }
    </div>,
};
