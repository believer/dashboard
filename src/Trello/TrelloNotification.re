let component = ReasonReact.statelessComponent("TrelloNotification");

let make = (~isLast, ~item: Trello.notification, _children) => {
  ...component,
  render: _self =>
    switch (item.data.listBefore, item.data.listAfter) {
    | (Some(before), Some(after)) =>
      <TrelloNotificationMessage
        icon="arrow"
        item
        isLast
        text={before.name ++ " -> " ++ after.name}
      />
    | (Some(_), None)
    | (None, Some(_))
    | (None, None) =>
      switch (item.data.text) {
      | Some(text) =>
        <TrelloNotificationMessage icon="message" isLast item text />
      | None => ReasonReact.null
      }
    },
};
