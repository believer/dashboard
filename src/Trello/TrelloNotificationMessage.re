let component = ReasonReact.statelessComponent("TrelloNotificationMessage");

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
  ++ item.creator.fullName
  ++ " "
  ++ {js|•|js}
  ++ " "
  ++ DateFns.format(item.date, "HH:mm")
  |> Utils.str;

let make =
    (
      ~isLast,
      ~item: Trello.notification,
      ~markAsRead,
      ~text,
      ~icon,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
      <div className="mr4">
        {
          switch (icon) {
          | "message" => <IconMessageCircle />
          | "arrow" => <IconArrowRight />
          | "archived" => <IconArchive />
          | _ => ReasonReact.null
          }
        }
        <IconCheckCircle className="mt2 green" onClick=markAsRead />
      </div>
      <div>
        <div className="mb2 lh-copy"> {text |> Utils.str} </div>
        <div className="f6 mid-gray">
          {
            switch (item.data.card) {
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
          item->trelloMessage
        </div>
      </div>
    </div>,
};
