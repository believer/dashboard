let component = ReasonReact.statelessComponent("TrelloNotification");

let make = (~item: Trello.notification, _children) => {
  ...component,
  render: _self =>
    <div className="flex mb3">
      <IconMessageCircle className="mr4" />
      <div>
        <div className="b mb2"> {item.data.text |> Utils.str} </div>
        <div className="f6 light-silver">
          {item.creator.fullName ++ " " ++ {js|•|js} ++ " " |> Utils.str}
          {
            MomentRe.moment(item.date)
            |> MomentRe.Moment.format("YYYY-MM-DD HH:mm")
            |> Utils.str
          }
        </div>
      </div>
    </div>,
};
