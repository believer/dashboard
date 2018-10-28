let component = ReasonReact.statelessComponent("TrelloIcon");

let make = (~listBefore, ~listAfter, ~card, ~text, _children) => {
  ...component,
  render: _self =>
    switch (listBefore, listAfter, card) {
    | (Some(_), Some(_), _) => <IconArrowRight />
    | (None, None, Some(c)) =>
      switch (text, c##closed) {
      | (Some(_), _) => <IconMessageCircle />
      | (None, Some(true)) => <IconArchive />
      | (_, _) => ReasonReact.null
      }
    | (_, _, _) => ReasonReact.null
    },
};
