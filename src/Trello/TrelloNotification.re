let component = ReasonReact.statelessComponent("TrelloNotification");

let make = (~isLast, ~item: Trello.notification, ~markAsRead, _children) => {
  ...component,
  render: _self =>
    switch (item.data.listBefore, item.data.listAfter) {
    | (Some(before), Some(after)) =>
      <TrelloNotificationMessage
        icon="arrow"
        item
        isLast
        markAsRead
        text={before.name ++ " -> " ++ after.name}
      />
    | (Some(_), None)
    | (None, Some(_))
    | (None, None) =>
      switch (item.data.text) {
      | Some(text) =>
        <TrelloNotificationMessage
          icon="message"
          isLast
          item
          markAsRead
          text
        />
      | None =>
        switch (item.data.card) {
        | Some(card) =>
          card.closed ?
            <TrelloNotificationMessage
              icon="archived"
              isLast
              item
              markAsRead
              text={card.name}
            /> :
            ReasonReact.null
        | None => ReasonReact.null
        }
      }
    },
};
