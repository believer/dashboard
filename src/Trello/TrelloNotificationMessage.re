let component = ReasonReact.statelessComponent("TrelloNotificationMessage");

let make = (~isLast, ~item: Trello.notification, ~text, ~icon, _children) => {
  ...component,
  render: _self =>
    <div className={Cn.make(["flex", "mb3"->Cn.ifTrue(!isLast)])}>
      {
        icon === "message" ?
          <IconMessageCircle className="mr4" /> : ReasonReact.null
      }
      {
        icon === "arrow" ? <IconArrowRight className="mr4" /> : ReasonReact.null
      }
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
          {
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
            |> Utils.str
          }
          {DateFns.format(item.date, "YYYY-MM-DD HH:mm") |> Utils.str}
        </div>
      </div>
    </div>,
};
