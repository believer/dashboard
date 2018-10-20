let component = ReasonReact.statelessComponent("TrelloNotificationMessage");

let make = (~isLast, ~text, ~name, ~date, ~icon, _children) => {
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
        <div className="mb2"> {text |> Utils.str} </div>
        <div className="f6 light-silver">
          {name ++ " " ++ {js|•|js} ++ " " |> Utils.str}
          {
            MomentRe.moment(date)
            |> MomentRe.Moment.format("YYYY-MM-DD HH:mm")
            |> Utils.str
          }
        </div>
      </div>
    </div>,
};
