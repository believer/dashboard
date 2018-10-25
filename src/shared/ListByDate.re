type listType =
  | GitHubList
  | TrelloList;

let component = ReasonReact.statelessComponent("ListByDate");

let make = (~index, ~title, ~totalItems, children) => {
  ...component,
  render: _self =>
    <>
      <h3 className="f6 mb0 mt0"> title->Utils.str </h3>
      <div
        className={
          Cn.make([
            "mt4",
            "mb4 pb4 bb bw-1 b--black-10"
            ->Cn.ifTrue(totalItems - 1 !== index),
          ])
        }>
        ...children
      </div>
    </>,
};
