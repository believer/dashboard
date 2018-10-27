let component = ReasonReact.statelessComponent("TrelloNotification");

let trelloMessage = (item: Trello.notification) =>
  (
    switch (item.data.board) {
    | Some(b) => " " ++ {js|•|js} ++ " " ++ b.name
    | None => ""
    }
  )
  ++ " "
  ++ {js|•|js}
  ++ " "
  ++ (
    switch (item.creator) {
    | Some(c) => c.fullName ++ " "
    | None => ""
    }
  )
  ++ {js|•|js}
  ++ " "
  ++ DateFns.format(item.date, "HH:mm")
  |> Utils.str;

let make = (~isLast, ~item: Trello.notification, ~markAsRead, _children) => {
  ...component,
  render: _self => {
    let {data}: Trello.notification = item;
    let {attachment, card, listBefore, listAfter, text}: Trello.notificationData = data;

    <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
      <div className="mr4">
        <TrelloIcon card listAfter listBefore text />
        <IconCheckCircle className="pointer mt2 green" onClick=markAsRead />
      </div>
      <div className="f6">
        {
          switch (card) {
          | Some(c) => <div className="b mb2 lh-copy"> c.name->Utils.str </div>
          | None => ReasonReact.null
          }
        }
        <TrelloText attachment listAfter listBefore text />
        <div className="mid-gray">
          {
            switch (card) {
            | Some(card) =>
              <a
                className="link dark-blue hover-hot-pink"
                href={"https://trello.com/c/" ++ card.shortLink}
                target="_blank">
                {"Link" |> Utils.str}
              </a>
            | None => ReasonReact.null
            }
          }
          {trelloMessage(item)}
        </div>
      </div>
    </div>;
  },
};
