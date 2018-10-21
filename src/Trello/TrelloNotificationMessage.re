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
          <a
            className="link navy hover-hot-pink"
            href={"https://trello.com/c/" ++ item.data.card.shortLink}
            target="_blank">
            {"Link" |> Utils.str}
          </a>
          {
            " "
            ++ {js|•|js}
            ++ " "
            ++ item.data.board.name
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
