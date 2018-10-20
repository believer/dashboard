let component = ReasonReact.statelessComponent("TrelloNotification");

let make = (~isLast, ~item: Trello.notification, _children) => {
  ...component,
  render: _self =>
    switch (item.data.listBefore, item.data.listAfter) {
    | (Some(before), Some(after)) =>
      <TrelloNotificationMessage
        date={item.date}
        icon="arrow"
        isLast
        name={item.creator.fullName}
        text={before.name ++ " -> " ++ after.name}
      />
    | (Some(_), None)
    | (None, Some(_))
    | (None, None) =>
      switch (item.data.text) {
      | Some(text) =>
        <TrelloNotificationMessage
          date={item.date}
          icon="message"
          isLast
          name={item.creator.fullName}
          text
        />
      | None => ReasonReact.null
      }
    },
};
